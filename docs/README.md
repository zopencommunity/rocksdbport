# RocksDB z/OS Port — Verification Guide

This directory contains test programs and build instructions for verifying
the RocksDB z/OS port after installation via `zopen build -vv`.

## 1. Source the installed environment

```sh
cd $HOME/zopen/usr/local/zopen/rocksdb/rocksdb-main 
. ./setup.sh
```

This sets `ZOPEN_EXTRA_CXXFLAGS`, `ZOPEN_EXTRA_LDFLAGS`, and `ZOPEN_EXTRA_LIBS`
to the values needed to compile and link against the installed `librocksdb.a`.

## 2. Smoke test — minimal Put/Get

`smoke.cc` opens a database, writes one key, reads it back, and prints the value.

```sh
# Compile
clang++ -std=gnu++20 -c $ZOPEN_EXTRA_CXXFLAGS /path/to/smoke.cc -o /path/to/smoke.o

# Link
clang++ -std=gnu++20 /path/to/smoke.o $ZOPEN_EXTRA_LDFLAGS $ZOPEN_EXTRA_LIBS -o /path/to/smoke

# Clean any leftover DB from a previous run, then execute
rm -rf /tmp/rocksdb_smoke_test
/path/to/smoke
```

Expected output:
```
smoke_test: got key k1 = v1
```

## 3. Extended example — WriteBatch, Delete, PinnableSlice

`simple_example.cc` exercises a wider set of the RocksDB API and uses
`assert()` to verify every operation.

```sh
# Compile
clang++ -std=gnu++20 -c $ZOPEN_EXTRA_CXXFLAGS /path/to/simple_example.cc -o /path/to/simple_example.o

# Link
clang++ -std=gnu++20 /path/to/simple_example.o $ZOPEN_EXTRA_LDFLAGS $ZOPEN_EXTRA_LIBS -o /path/to/simple_example

# Clean any leftover DB from a previous run, then execute
rm -rf /tmp/rocksdb_simple_example
/path/to/simple_example
```

Expected output:
```
simple_example: all assertions passed
```

## 4. Verify the created database on disk

After running either test you can inspect the DB files directly:

```sh
# List the files written by the smoke test
ls -l /tmp/rocksdb_smoke_test/

# List the files written by the simple_example test
ls -l /tmp/rocksdb_simple_example/
```

Expected contents (file names will vary by run):
```
CURRENT        -- points to the active MANIFEST
IDENTITY       -- unique DB identifier (UUID)
LOCK           -- advisory lock file
MANIFEST-*     -- version-set log
OPTIONS-*      -- snapshot of DB options at open time
*.sst          -- SST data file (written by the explicit Flush() call)
*.log          -- write-ahead log (WAL)
```

Use `ldb` to dump the key-value pairs from a DB:

```sh
ldb --db=/tmp/rocksdb_smoke_test scan
```

Expected output:
```
k1 : v1
```

For the extended example (after `simple_example` runs, `key1` is deleted
and `key2` holds the value):

```sh
ldb --db=/tmp/rocksdb_simple_example scan
```

Expected output:
```
key2 : value
```

Use `sst_dump` to inspect an individual SST file if one is present:

```sh
# List SST files first, then pass the exact filename
ls /tmp/rocksdb_smoke_test/*.sst
sst_dump --file=/tmp/rocksdb_smoke_test/<filename>.sst --command=scan
```

## 5. Verify the installed tools

`ldb` and `sst_dump` are installed under `bin/`:

```sh
# ldb — command-line DB inspection tool
ldb --help

# sst_dump — SST file inspection tool
sst_dump --help
```

Both should print usage text without errors.

