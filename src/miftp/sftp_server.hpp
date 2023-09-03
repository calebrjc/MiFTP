#pragma once

#include <functional>
#include <unordered_map>
#include <yonaa/yonaa.hpp>

#include "sftp_request.hpp"
#include "sftp_response.hpp"

/// @brief Represents a session with a client.
struct sftp_session_info {};

/// @brief Represents a server that handles SFTP requests.
class sftp_server {
   public:
    /// @brief Constructs a new sftp_server on the given port.
    /// @param port The port to listen on.
    sftp_server(uint16_t port);

    /// @brief Handles the addition of a new client.
    /// @param client_id The id of the client that connected.
    void handle_client_connect(yonaa::client_id client_id);

    /// @brief Handles the removal of a client.
    /// @param client_id The id of the client that disconnected.
    void handle_client_disconnect(yonaa::client_id client_id);

    /// @brief Handles a request from a client.
    /// @param client_id The id of the client that sent the request.
    /// @param data The data sent by the client.
    void handle_request(yonaa::client_id client_id, const yonaa::buffer &data);

    /// @brief Handles a USER request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_USER(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles an ACCT request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_ACCT(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a PASS request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_PASS(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a TYPE request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_TYPE(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a LIST request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_LIST(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a CDIR request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_CDIR(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a KILL request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_KILL(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a NAME request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_NAME(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a TOBE request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_DONE(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a RETR request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_RETR(yonaa::client_id client_id, const sftp_request &request);

    /// @brief Handles a SEND request.
    /// @param client_id The id of the client that sent the request.
    /// @param request The request sent by the client.
    void handle_STOR(yonaa::client_id client_id, const sftp_request &request);

   private:
    using request_handler = std::function<void(yonaa::client_id, const sftp_request &)>;

   private:
    yonaa::server server_;
    std::unordered_map<sftp_request_type, request_handler> handlers_;
    std::unordered_map<yonaa::client_id, sftp_session_info> sessions_;
};
