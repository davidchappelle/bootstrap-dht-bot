#include "libtorrent/session.hpp"
#include "libtorrent/alert_types.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <mutex>
#include <stdlib.h>

using namespace libtorrent;

void usage()
{
	std::cerr << "USAGE: bootstrap-dht-bot <host> <port>" << std::endl;
	exit(1);
}


void dispatch_libtorrent_alert(std::shared_ptr<session> s, std::auto_ptr<libtorrent::alert> libtorrent_alert)
{
    std::cerr << "libtorrent alert: " << libtorrent_alert->message() << std::endl;
    switch (libtorrent_alert->type()) {
        case libtorrent::dht_bootstrap_alert::alert_type:
            std::cerr << "bootstrapping successful" << std::endl;
            break;
    }
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
        usage();
    }

    const char* host = argv[1];
    int port = atoi(argv[2]);
    int flags = 0;

    if (port == 0) {
        std::cerr << "error: port is invalid" << std::endl;
        exit(-1);
    }

    libtorrent::fingerprint fp("LT", LIBTORRENT_VERSION_MAJOR, LIBTORRENT_VERSION_MINOR, 0, 0);
    std::shared_ptr<libtorrent::session> s = std::make_shared<libtorrent::session>(
            fp, std::make_pair(port, port + 8), "0.0.0.0", flags,
            libtorrent::alert::port_mapping_notification |
            libtorrent::alert::debug_notification |
            libtorrent::alert::dht_notification |
            libtorrent::alert::error_notification |
            libtorrent::alert::status_notification);

    s->set_alert_dispatch(std::bind(&dispatch_libtorrent_alert, s, std::placeholders::_1));
    s->add_dht_router(std::make_pair(host, 6881));
    s->start_dht();

    // Just block here and let the session run forever.
    std::mutex forever;
    forever.lock();
    forever.lock();
}
