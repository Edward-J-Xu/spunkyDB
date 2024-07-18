# SpunkyDB
Creating a database from scratch in C++ for fun!

SpunkyDB is an open source database.

## Why use SpunkyDB?

SpunkyDB aims to be a high speed, modern implementation of the latest database-relevant algorithms. It aims to be configurable for a range of application and data safety use cases. Multiple abstraction layers over the low-level key-value store provide multi-model capability without sacrificing performance.

The database aims to run on every platforms from an 8-bit microcontroller for edge IoT applications all the way up to multi-node clusters hosted in the cloud, with multiple tenant organisations and applications. The design is highly componentised for this reason.

The database is still young, but I hope to have a proof of concept on a multi-model database with query support that runs on multiple operating systems during the second half of 2024.

## Getting Started

First, download QtCreator for your platform. Then, clone the repository and open the main `spunkydb.pro` project file in QtCreator. You can build the entire project from here. It's recommended to start with the command line interface (CLI) named `spunkydb-cli`.

## Features

This database is being developed focusing on database design and C++ features that I want to learn rather than being driven by traditional customer or user demands.

Currently, it offers the following user features:

- (All keys below are unicode strings - you can even use smilies!)
- (All set operations also allow the specification of which bucket to store the key within)
- Set a key-value pair (string key->string value)
- Set a key-value pair (string key->set-of-strings value)
- Retrieve a string value for a string key
- Retrieve a set-of-strings value for a string key
- Query the database for all keys in a named (string) bucket

And these administrative features:

- Create a new empty database
- Delete a database and all of its content
- Bucket indexing support (term list)

These data safety and storage features are present:-

- Strongly consistent file kv store (can be used as a data store or a query index store)
- Strongly consistent in-memory kv store (can be used as a data store or a query index store, and as a read cache for an underlying key-value store, such as the file kv store)


## Future Roadmap

While there's no strict roadmap, there are a few design principles I plan to follow:

- Multi-model: Will support various NoSQL, relational, and graph models.
- Advanced querying: Including full-text search, geospatial queries, and even DNA sequences!
- Enterprise grade (eventually): Designed to easily add ACID compliance and deep security features.
- Cloud ready: Will support both scale-out and scale-up configurations to suit your needs, and will be compatible with Kubernetes (when I get around to it).
- Staying current: Will keep up with the latest developments in modern C++ specifications and database theory.
- Configurable for diverse needs: From single-threaded embedded use cases to web-scale applications 😉, supporting everything from eventually consistent high-speed operations to ACID-compliant high durability.
