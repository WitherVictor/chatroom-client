#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/exception/exception.hpp>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <print>

using json = nlohmann::json;
namespace asio = boost::asio;

int main()
{
    std::ifstream config_json_fstream{ "config.json" };

    if (!config_json_fstream)
    {
        spdlog::info("Config file missing, trying to create one");
    }
    else
    {
        spdlog::info("Config file found");
    }

    json config_json = json::parse(config_json_fstream);

    //  从文件内读取 server 的 ip, port, username
    auto address_string = config_json["server ip"].get<std::string>();
    auto port_string = config_json["port"].get<std::string>();
    auto username = config_json["username"].get<std::string>();
    
    //  如果 Server ip 与 Port 任意一栏为空, 则抛出异常
    if (address_string.empty())
    {
        spdlog::critical("Server ip cannot be null !");
        throw std::logic_error{"Server ip adress read from config is null !"};
    }

    if (port_string.empty())
    {
        spdlog::critical("Server port cannot be null !");
        throw std::logic_error{"Server port read from config is null"};
    }

    if (username.empty())
    {
        spdlog::critical("User name cannot be null !");
        throw std::logic_error{"Username read from config is null"};
    }

    //  将 Server ip 转换成 ip::address
    auto address = asio::ip::address::from_string(address_string);    

    //  将 Port String 转换成数字
    std::uint_least16_t port{};
    std::from_chars(port_string.data(), port_string.data() + port_string.size(), port);

    //  定义 Socket
    asio::io_context io_context{};
    asio::ip::tcp::socket socket{ io_context };

    //  要连接到的目标 Server
    asio::ip::tcp::endpoint endpoint{address, port};

    //  连接到服务器
    socket.connect(endpoint);
    spdlog::info("Trying to connect to server: {}:{}", endpoint.address().to_string(), endpoint.port());


    if (socket.is_open())
        spdlog::info("Successfully connected, Username: {}", username);
    else
    {
        spdlog::critical("Connect failed !");
        throw std::logic_error{"Cannot connect to the server"};
    }

    //  接收信息的缓冲区 buffer
    constexpr std::size_t buffer_size{ 4096 };
    std::array<unsigned char, buffer_size> buffer{};

    while (true)
    {
        std::print("{} >> ", username);
        std::string message{};
        std::getline(std::cin, message);

        if (message == "/exit")
        {
            socket.close();
            break;
        }
        else if (message.empty())
            continue;
        
        socket.write_some(asio::buffer(message));

        auto read_length = socket.read_some(asio::buffer(buffer));
        std::string read_message{buffer.begin(), buffer.begin() + read_length};
        std::println("{} >> {}", username, read_message);
    }
}