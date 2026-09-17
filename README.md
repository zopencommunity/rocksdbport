[![Automatic version updates](https://github.com/zopencommunity/rocksdbport/actions/workflows/bump.yml/badge.svg)](https://github.com/ZOSOpenTools/rocksdbport/actions/workflows/bump.yml)

# rocksdb

This code is a library that forms the core building block for a fast key-value server, especially suited for storing data on flash drives. It has a Log-Structured-Merge-Database (LSM) design with flexible tradeoffs between Write-Amplification-Factor (WAF), Read-Amplification-Factor (RAF) and Space-Amplification-Factor (SAF). It has multi-threaded compactions, making it especially suitable for storing multiple terabytes of data in a single database.

# Installation and Usage

Use the zopen package manager ([QuickStart Guide](https://zopen.community/#/Guides/QuickStart)) to install:
```bash
zopen install rocksdb
```

# Building from Source

1. Clone the repository:
```bash
git clone https://github.com/zopencommunity/rocksdbport.git
cd rocksdbport
```
2. Build using zopen:
```bash
zopen build -vv
```

See the [zopen porting guide](https://zopen.community/#/Guides/Porting) for more details.

# Documentation


# Troubleshooting

# Contributing

Contributions are welcome! Please follow the [zopen contribution guidelines](https://github.com/zopencommunity/meta/blob/main/CONTRIBUTING.md).
