// Copyright (c) Meta Platforms, Inc. and affiliates.
// This source code is licensed under both the GPLv2 (found in the
// COPYING file in the root directory) and Apache 2.0 License
// (found in the LICENSE.Apache file in the root directory).
//
// Minimal smoke test for the z/OS RocksDB port.
// Opens a DB, writes one key, reads it back, flushes to SST, and prints
// the value.  The explicit Flush() ensures ldb scan can read the key after
// the program exits (ldb reads SST files, not the WAL).

#include <iostream>
#include <memory>
#include <string>

#include "rocksdb/db.h"
#include "rocksdb/options.h"

int main() {
  rocksdb::Options options;
  options.create_if_missing = true;

  std::unique_ptr<rocksdb::DB> db;
  rocksdb::Status s =
      rocksdb::DB::Open(options, "/tmp/rocksdb_smoke_test", &db);
  if (!s.ok()) {
    std::cerr << "Open failed: " << s.ToString() << "\n";
    return 1;
  }

  s = db->Put(rocksdb::WriteOptions(), "k1", "v1");
  if (!s.ok()) {
    std::cerr << "Put failed: " << s.ToString() << "\n";
    return 1;
  }

  std::string value;
  s = db->Get(rocksdb::ReadOptions(), "k1", &value);
  if (!s.ok()) {
    std::cerr << "Get failed: " << s.ToString() << "\n";
    return 1;
  }

  // Flush memtable to an SST file so that `ldb scan` can read the key
  // after this process exits (ldb does not read the WAL).
  s = db->Flush(rocksdb::FlushOptions());
  if (!s.ok()) {
    std::cerr << "Flush failed: " << s.ToString() << "\n";
    return 1;
  }

  std::cout << "smoke_test: got key k1 = " << value << "\n";
  return 0;
}

