bootstrap-dht-bot
=================

DHT bootstrap bots can be used to prime a DHT bootstrap server with
a base set of nodes.

The command line options are:

	usage: bootstrap-dht-bot <host> <port>

The first argument when launching the bot is the server host name or ip
address. The second argument provides the port on which the server is
listening.

building
--------

To build, you need boost_ and unpack it to a path specified by ``$BOOST_ROOT``
and *boost-build*. Build by running: ``b2``.

.. _boost: http://www.boost.org

If you prefer to build manually, just compile ``main.cpp`` and link against
the boost.system library as well as adding boost to your include path.

To build you need a C++ 11 enabled compiler.

