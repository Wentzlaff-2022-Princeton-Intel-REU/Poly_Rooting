#!/usr/bin/env node

import stream from 'node:stream';
import fs from 'node:fs';
import os from 'node:os';
import readline from 'node:readline';
import child_process from 'node:child_process';
import pLimit from 'p-limit';

console.log(`Running with concurrency of ${os.cpus().length}`);
const limit = pLimit(os.cpus().length);

process.on('uncaughtException', function (err) {
    if (err.code === 'EPIPE')
        return;
    console.error(err.stack);
    process.exit(4);
});

const prog = process.argv[2];
const suite = process.argv[3];
if (process.argv.length !== 4 && process.argv.length !== 5) {
    console.error('Usage: ./test/run <path/to/executable> <suite> [<testId>]');
    process.exit(2);
}
fs.accessSync(prog, fs.constants.R_OK | fs.constants.X_OK);

class TestCase {
    constructor(id, line) {
        this.testId = id;
        const sp = line.split(' ').filter((s) => s).map((s) => +s);
        this.n = sp.splice(0, 1)[0];
        this.coeffs = sp.splice(0, this.n + 1);
        this.roots = sp;
        this.isPassed = false;
        this.isFailed = false;
        this.isErrored = false;
        this.description = `Test #${this.testId} (n=${this.n}): `;
        this.stderr = '';
        this.stdout = '';
        this.exitCode = -1;
        this.signalCode = '';
        this.reportedRoots = [];
        if (this.roots.length > this.n)
            throw new Error(`Wrong test case ${line}`);
    }

    async run() {
        const rst = await new Promise((resolve, reject) => {
            const cp = child_process.execFile(prog, ['1e-10'], {
                timeout: 1000,
                killSignal: 'SIGABRT',
            }, (err, stdout, stderr) => {
                this.stdout = stdout;
                this.stderr = stderr;
                this.signalCode = cp.signalCode;
                this.exitCode = cp.exitCode;
                if (err) {
                    resolve(err);
                } else {
                    const rst = stdout.split(/\s+/).filter((s) => s).map((s) => +s).filter((v) => !isNaN(v));
                    resolve(rst);
                }
            });
            try {
                const stdinStream = new stream.Readable();
                stdinStream.push(`${this.n}\n`);
                for (const c of this.coeffs)
                    stdinStream.push(`${c}\n`);
                stdinStream.push(null);
                stdinStream.pipe(cp.stdin);
            } catch (e) {
                // ignore
            }
        });

        if (this.isConnected) {
            console.log('========= STDERR =========');
            console.log(this.stderr);
            console.log('========= Exit Code =========');
            if (this.signalCode)
                console.log(''+this.exitCode, `(${this.signalCode})`);
            else
                console.log(''+this.exitCode);
            console.log('========= STDOUT =========');
            console.log(this.stdout);
            console.log('========= Expected =========');
            console.log(this.roots.join(' '));
            console.log('========= Summary =========');
        }
        if (this.signalCode === 'SIGABRT') {
            this.description += 'Timeout';
            this.isErrored = true;
            return;
        }
        if (!Array.isArray(rst)) {
            this.description += `Killed by ${this.signalCode}`;
            this.isErrored = true;
            return;
        }
        if (!rst.length) {
            this.description += 'No root reported';
            this.isFailed = true;
            return;
        }
        this.reportedRoots = rst;
        let goodRoots = 0, badRoots = 0;
        const numErrors = [];
        for (const e of rst) {
            const r = this.roots.find((r) => Math.abs(e - r) < 1e-7);
            if (r === undefined)
                badRoots++;
            else
                goodRoots++, numErrors.push(Math.abs(e - r));
        }
        if (!goodRoots) {
            this.description += `${rst.length}/${roots.length} roots reported but all bad`;
            this.isFailed = true;
            return;
        }
        const maxErr = numErrors.reduce((a, b) => Math.max(a, b));
        const avgErr = numErrors.reduce((a, b) => a + b) / goodRoots;
        if (goodRoots < this.roots.length) {
            this.description += `${rst.length} reported, ${goodRoots} valid, err:  ${avgErr} avg.  ${maxErr} max.`;
            this.isFailed = true;
            return false;
        }
        if (badRoots) {
            this.description += `all ${this.roots.length} found but ${badRoots} fake reported, err:  ${avgErr} avg.  ${maxErr} max.`;
            this.isFailed = true;
            return false;
        }
        this.description += `all ${this.roots.length} found, err:  ${avgErr} avg.  ${maxErr} max.`;
        this.isPassed = true;
        return true;
    }
}

async function processLineByLine(fn) {
    const fileStream = fs.createReadStream(fn);
    const rl = readline.createInterface({
        input: fileStream,
        crlfDelay: Infinity,
    });
    const cases = [];
    const runners = [];
    let testId = 1;
    let passed = 0, failed = 0, errored = 0;
    for await (const line of rl) {
        if (process.argv[4] && testId !== +process.argv[4]) {
            testId++;
            continue;
        }
        const tc = new TestCase(testId++, line);
        tc.isConnected = !!process.argv[4];
        cases.push(tc);
        runners.push(limit(async () => {
            await tc.run();
            if (tc.isPassed)
                passed++;
            if (tc.isFailed)
                failed++;
            if (tc.isErrored)
                errored++;
            console.log(tc.description);
        }));
    }
    await Promise.all(runners);
    console.error(`Total Pass: ${passed}  Total Fail: ${failed}  Total Error: ${errored}`);
}

processLineByLine(new URL(`./data/${suite}.txt`, import.meta.url));
