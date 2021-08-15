#include <ctime>
#include <iostream>
#include <string>
#include <functional>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
    time_t now = std::time(0);
    return std::ctime(&now);
}

class Connection:
    public std::enable_shared_from_this<Connection> {
    std::string message;
public:
    tcp::socket socket;

    Connection(boost::asio::io_context & io_context): socket(io_context) {}

    typedef std::shared_ptr<Connection> ptrConnection;

    static ptrConnection Create(boost::asio::io_context & io_context) {
        return ptrConnection(new Connection(io_context));
    }

    void Start() {
        message = make_daytime_string();

        auto handler = std::bind(&Connection::HandleWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
        socket.async_write_some(boost::asio::buffer(message), handler);
    }

    void HandleWrite(boost::system::error_code ec, std::size_t length) {

    }
};

class Server {
    boost::asio::io_context & io_context;
    tcp::acceptor acceptor;
public:
    Server(boost::asio::io_context & io_context):
        io_context(io_context),
        acceptor(io_context, tcp::endpoint(tcp::v4(), 13)) {
        StartAccept();
    }

    void StartAccept() {
        Connection::ptrConnection connection = Connection::Create(io_context);
        
        auto handler = std::bind(&Server::HandleAccept, this, connection, std::placeholders::_1);
        acceptor.async_accept(connection->socket, handler);
    }

    void HandleAccept(Connection::ptrConnection connection, boost::system::error_code ec) {
        if (!ec) {
            connection->Start();
        }
        StartAccept();
    }
};

int main(int argc, char** argv) {
    try {
        boost::asio::io_context io_context;
        Server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
