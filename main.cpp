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
	std::cerr << "USAGE: bootstrap-dht-bot <server-host> <server-port> <session-port>" << std::endl;
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
            libtorrent::alert::port_mapping_notification |
            libtorrent::alert::debug_notification |
            libtorrent::alert::dht_notification |
            libtorrent::alert::error_notification |
            libtorrent::alert::status_notification);

    session->set_alert_dispatch(std::bind(&dispatch_libtorrent_alert, session, std::placeholders::_1));
    session->add_dht_router(std::make_pair(server_host, server_port));
    session->start_dht();

    // Just block here and let the session run forever.
    std::mutex forever;
    forever.lock();
    forever.lock();

    std::cerr << "stopping bot" << std::endl;

    return 0;
}
