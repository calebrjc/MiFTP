#pragma once

#include <functional>
#include <unordered_map>
#include <yonaa/yonaa.hpp>

#include "sftp/request.hpp"
#include "sftp/response.hpp"

namespace sftp {

/// @brief Represents a session with a client.
struct session_info {};

/// @brief Represents a server that handles SFTP requests.
class server {
   public:
    /// @brief Constructs a new server on the given port.
    /// @param port The port to listen on.
    server(uint16_t port);

    /// @brief Run the server.
    void run();

    /// @brief Stop the server.
    void stop();

   private:
    using request_handler = std::function<void(yonaa::client_id, const request &)>;

   private:
    void send_response(yonaa::client_id client_id, const response &response);

    void handle_client_connect(yonaa::client_id client_id);
    void handle_client_disconnect(yonaa::client_id client_id);
    void handle_request(yonaa::client_id client_id, const yonaa::buffer &data);

    void handle_user(yonaa::client_id client_id, const request &request);
    void handle_acct(yonaa::client_id client_id, const request &request);
    void handle_pass(yonaa::client_id client_id, const request &request);
    void handle_type(yonaa::client_id client_id, const request &request);
    void handle_list(yonaa::client_id client_id, const request &request);
    void handle_cdir(yonaa::client_id client_id, const request &request);
    void handle_kill(yonaa::client_id client_id, const request &request);
    void handle_name(yonaa::client_id client_id, const request &request);
    void handle_done(yonaa::client_id client_id, const request &request);
    void handle_retr(yonaa::client_id client_id, const request &request);
    void handle_stor(yonaa::client_id client_id, const request &request);

   private:
    yonaa::server server_;
    std::unordered_map<request_type, request_handler> handlers_;
    std::unordered_map<yonaa::client_id, session_info> sessions_;
};

}  // namespace sftp
