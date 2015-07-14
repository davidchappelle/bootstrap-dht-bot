#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/session_settings.hpp>
#include <memory>
#include <mutex>
#include <stdlib.h>

void usage()
{
	std::cerr << "USAGE: bootstrap-dht-bot <server-host> <server-port> <session-port>" << std::endl;
	exit(1);
}

void dispatch_libtorrent_alert(
        std::shared_ptr<libtorrent::session> s,
        std::auto_ptr<libtorrent::alert> libtorrent_alert)
{
    std::cerr << "libtorrent alert: " << libtorrent_alert->message() << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        usage();
    }

    const char* server_host = argv[1];
    int server_port = atoi(argv[2]);
    int session_port = atoi(argv[3]);
    int flags = 0;

    if (server_port == 0) {
        std::cerr << "error: server port '" << server_port << "' is invalid" << std::endl;
        exit(-1);
    }

    if (session_port == 0) {
        std::cerr << "error: session port '" << session_port << "' is invalid" << std::endl;
        exit(-1);
    }

    libtorrent::fingerprint fp("LT", LIBTORRENT_VERSION_MAJOR, LIBTORRENT_VERSION_MINOR, 0, 0);
    std::shared_ptr<libtorrent::session> session = std::make_shared<libtorrent::session>(
            fp, std::make_pair(session_port, session_port + 8), "0.0.0.0", flags,
            libtorrent::alert::all_categories |
            libtorrent::alert::debug_notification |
            libtorrent::alert::dht_notification |
            libtorrent::alert::error_notification |
            libtorrent::alert::status_notification |
            libtorrent::alert::tracker_notification);

    session->set_alert_dispatch(std::bind(&dispatch_libtorrent_alert, session, std::placeholders::_1));
    session->add_dht_router(std::make_pair(server_host, server_port));

    // Cusomized dht settings.
    libtorrent::dht_settings dht_settings;

    // Allow interacting with class A networks. This is to deal with the case where
    // the bootstrap router and/or bots are running in a class A networki that is
    // no longer considered to be dark/private.
    dht_settings.ignore_dark_internet = false;

    // Allow multiple bots on the same host to co-exist in the routing table buckets.
    dht_settings.restrict_routing_ips = false;

    session->set_dht_settings(dht_settings);

    session->start_dht();

    // Just block here and let the session run forever.
    std::mutex forever;
    forever.lock();
    forever.lock();

    return 0;
}
