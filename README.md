SOS - Scalable Object Store
===========================

SOS (pronuounced "sôs") -- Scalable Object Store -- is a high-performance,
indexed, object-oriented database designed to efficiently manage structured data
on persistent media.

An **object** is a collection of attribute values conforming an object
**schema**, which defines type of attributes, in a SOS container.

A SOS **container** is a place where SOS users can insert or retrieve objects. A
SOS container comprises one or more **partitions**, in which objects are stored.
Users can then easily manage a collection of objects by partition, e.g.  old
object archival.

`sos_cmd` is the CLI program to:
* create SOS containers.
* add schema to the container.
* add/import objects into the container.
* query objects from the container.

Please see `sos_cmd -h` for more information about how to use `sos_cmd`.

The following list of programs are for partition management:
* `sos_part_create` for creating a partition in a container.
* `sos_part_modify` for modifying partition status (`PRIMARY`, `ACTIVE`,
  `OFFLINE`).
* `sos_part_query` for listing partitions and their statuses in a container.
* `sos_part_delete` for deleting a partition from a container.

For more details about SOS documentation, please see [SOS
doc](http://www.opengridcomputing.com/sos_doc/index.html), which also contains
SOS C API.


Dependencies
------------

* libyaml
* libyaml-dev
* For SOS python interface:
    * swig
    * Python-2.7


Installation
------------

```sh
./autogen.sh # this will call autoreconf to generate `configure` script
mkdir build
cd build
../configure --prefix=/SOS/INSTALL/PATH [--enable-swig]
# add `--enable-swig` for SOS python interface
make && make install
```
