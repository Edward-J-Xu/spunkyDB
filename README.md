# SpunkyDB
Creating a database from scratch in C++ for fun!

SpunkyDB is an open source database.

## Getting Started

First, download QtCreator for your platform. Then, clone the repository and open the main `spunkydb.pro` project file in QtCreator. You can build the entire project from here. It's recommended to start with the command line interface (CLI) named `spunkydb-cli`.

## Features

This database is being developed focusing on database design and C++ features that I want to learn rather than being driven by traditional customer or user demands.

Currently, it offers the following user features:

- Set a key-value pair
- Retrieve a value for a key

(Hey, you gotta start somewhere!)

And these administrative features:

- Create a new empty database
- Delete a database and all of its content

## Future Roadmap

While there's no strict roadmap, there are a few design principles I plan to follow:

- Multi-model: Will support various NoSQL, relational, and graph models.
- Advanced querying: Including full-text search, geospatial queries, and even DNA sequences!
- Enterprise grade (eventually): Designed to easily add ACID compliance and deep security features.
- Cloud ready: Will support both scale-out and scale-up configurations to suit your needs, and will be compatible with Kubernetes (when I get around to it).
- Staying current: Will keep up with the latest developments in modern C++ specifications and database theory.
- Configurable for diverse needs: From single-threaded embedded use cases to web-scale applications 😉, supporting everything from eventually consistent high-speed operations to ACID-compliant high durability.

