#include "sftp/server.hpp"

#include <iostream>

namespace sftp {

server::server(uint16_t port) : server_(port) {
    // Mold handlers into yonaa::server handler types
    auto connect_handler      = [this](yonaa::client_id id) { handle_client_connect(id); };
    auto disconnect_handler   = [this](yonaa::client_id id) { handle_client_disconnect(id); };
    auto data_receive_handler = [this](yonaa::client_id id, const yonaa::buffer &data) {
        handle_request(id, data);
    };

    // Install the handlers on our server
    server_.set_client_connect_handler(connect_handler);
    server_.set_client_disconnect_handler(disconnect_handler);
    server_.set_data_receive_handler(data_receive_handler);

    // Create handler map
    //
    // I would like for this map to be static, but because the handlers are member funcion, we need
    // a reference to `this`
    using namespace std::placeholders;
    // clang-format off
    handlers_ = {
        // Insert a NOP for NONE
        {request_type::none, [](yonaa::client_id id, const request &r) { (void)id; (void)r; }},
        
        {request_type::user, std::bind(&server::handle_user, this, _1, _2)},
        {request_type::acct, std::bind(&server::handle_acct, this, _1, _2)},
        {request_type::pass, std::bind(&server::handle_pass, this, _1, _2)},
        {request_type::type, std::bind(&server::handle_type, this, _1, _2)},
        {request_type::list, std::bind(&server::handle_list, this, _1, _2)},
        {request_type::cdir, std::bind(&server::handle_cdir, this, _1, _2)},
        {request_type::kill, std::bind(&server::handle_kill, this, _1, _2)},
        {request_type::name, std::bind(&server::handle_name, this, _1, _2)},
        {request_type::tobe, std::bind(&server::handle_name, this, _1, _2)},
        {request_type::done, std::bind(&server::handle_done, this, _1, _2)},
        {request_type::retr, std::bind(&server::handle_retr, this, _1, _2)},
        {request_type::send, std::bind(&server::handle_retr, this, _1, _2)},
        {request_type::stop, std::bind(&server::handle_retr, this, _1, _2)},
        {request_type::stor, std::bind(&server::handle_stor, this, _1, _2)},
        {request_type::size, std::bind(&server::handle_stor, this, _1, _2)},
    };
    // clang-format on
}

void server::run() {
    server_.run();
}

void server::stop() {
    server_.stop();
}

void server::handle_client_connect(yonaa::client_id client_id) {
    (void)client_id;

    YONAA_DEBUG("Client {} connected", client_id);
    std::cout << fmt::format("Client {} connected\n", client_id);
}

void server::handle_client_disconnect(yonaa::client_id client_id) {
    (void)client_id;

    YONAA_DEBUG("Client {} disconnected", client_id);
}

void server::handle_request(yonaa::client_id client_id, const yonaa::buffer &data) {
    // Transform the raw data into a sensible request
    auto raw_bytes                 = std::string_view(data.data(), data.size());
    std::optional<request> request = request::from_bytes(raw_bytes);

    // If the data can't be made into a valid request, assume that this is a client that we don't
    // want to communicate with
    if (!request) {
        YONAA_DEBUG("Client {} sent an invalid request", client_id);
        server_.remove_client(client_id);
        return;
    }

    // Get the handler for this type of request
    request_handler handler_function = handlers_[request->type];

    // If there is a pending request, use that request type instead
    session_info session = sessions_[client_id];
    (void)session;
    /*
    if (session.pending_request) {
        handler_function = handlers_[session.pending_request.type];
    }
    */

    // There should be a handler registered for every request type. If there is a type that does not
    // have a handler, that is a programming error.
    assert(handler_function);

    // Call the handler
    handler_function(client_id, *request);
}

void server::handle_user(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling USER request");

    // Success pre-conditions:
    // - arg count = 1
    // - client is not logged in
    // - db contains specified username

    /*
    if request.args.size() != 1
        return -

    if client.logged_in:
        return !  // User already logged in

    if not db.contains(request.args[0]):
        return -  // User does not exist

    client.username = request.args[0]

    if client.password.empty():
        return +  // User ok, send password

    return !
    */
}

void server::handle_acct(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling ACCT request");

    /*
    if request.args.size() != 1
        return -

    return + // no effect, feature not implemented
    */
}

void server::handle_pass(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling PASS request");

    // Success pre-conditions:
    // - arg count = 1
    // - client is not logged in
    // - A username is specified
    // - password is correct

    /*
    if request.args.size() != 1
        return -

    if client.logged_in:
        return !  // User already logged in

    if not client.username:
        return -  // No user specified

    if db.get_client(client.username).check_password():
        return !  // Correct password; user logged in

    return -  // Wrong password
    */
}

void server::handle_type(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling TYPE request");

    /*
    if not client.logged_in:
        return -  // User not logged in

    return +  // No effect; only binary mode is supported
    */
}

void server::handle_list(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling LIST request");

    // Success pre-conditions:
    // - the client is logged in
    // - arg count = 2
    // - args[0] is 'F' or 'V'
    // - args[1] is a valid directory
    // - args[1] is accessible by the user

    /*
    if request.args.size() != 2
        return -

    if not client.logged_in:
        return -  // User not logged in

    if request.args[0] != 'F' and request.args[0] != 'V':
        return -  // Invalid list type

    path = request.args[1]
    dirent = std::directory_entry(path)

    if not dirent.exists() or not dirent.is_directory():
        return -  // Invalid directory

    if not dirent.is_accessible_by(client.username):
        return -  // Directory not accessible by user

    ... format directory data ...

    return !  // Directory data
    */
}

void server::handle_cdir(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling CDIR request");

    // Success pre-conditions:
    // - the client is logged in
    // - arg count = 1
    // - args[0] is a valid directory
    // - args[0] is accessible by the user

    /*
    if request.args.size() != 1
        return -

    if not client.logged_in:
        return -  // User not logged in

    new_dir_path = request.args[0]
    dirent = std::directory_entry(new_dir_path)

    if not dirent.exists() or not dirent.is_directory():
        return -  // Invalid directory

    if not dirent.is_accessible_by(client.username):
        return -  // Directory not accessible by user

    client.current_dir = new_dir_path
    return +  // Success
    */
}

void server::handle_kill(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling KILL request");

    // Success pre-conditions:
    // - the client is logged in
    // - arg count = 1
    // - args[0] exists
    // - args[0] is accessible by the user

    /*
    if request.args.size() != 1
        return -

    if not client.logged_in:
        return -  // User is not logged in

    path = request.args[0]
    dirent = std::directory_entry(path)

    if not dirent.exists():
        return -  // File does not exist

    if not dirent.is_accessible_by(client.username):
        return -  // File not accessible by user

    if not dirent.is_regular_file():
        return -  // Not a file

    dirent.remove()
    return +  // Success
    */
}

void server::handle_name(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    switch (request.type) {
        case request_type::name:
            YONAA_DEBUG("Handling NAME request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 1
            // - there is no pending request
            // - args[0] exists
            // - args[0] is accessible by the user

            /*
            if request.args.size() != 1
                return -

            if not client.logged_in:
                return -  // User is not logged in

            if client.pending_request:
                return -  // Pending request

            path = request.args[0]
            dirent = std::directory_entry(path)

            if not dirent.exists() or not dirent.is_directory():
                return -  // Invalid directory

            if not dirent.is_accessible_by(client.username):
                return -  // Directory not accessible by user

            client.pending_request = request
            return +  // Success
            */

        case request_type::tobe:
            YONAA_DEBUG("Handling TOBE request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 1
            // - there is a pending rename request

            /*
            if request.args.size() != 1
                return -

            if not client.logged_in:
                return -  // User is not logged in

            if not client.pending_request and client.pending_request.type !=
            request_type::name: return -  // No pending rename request

            std::filesystem::rename(client.pending_request.args[0], request.args[0])
            client.pending_request = std::nullopt
            return +  // Success
            */

        default:
            assert(0);
    }
}

void server::handle_done(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    YONAA_DEBUG("Handling DONE request");

    // Success pre-conditions:
    // - arg count = 0

    /*
    if request.args.size() != 0
        return -

    return +  // Success
    */
}

void server::handle_retr(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    switch (request.type) {
        case request_type::retr:
            YONAA_DEBUG("Handling RETR request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 1
            // - there is no currently pending request
            // - args[0] exists
            // - args[0] is accessible by the user

            /*
            if request.args.size() != 1
                return -

            if not client.logged_in:
                return -  // User is not logged in

            if client.pending_request:
                return -  // Pending request

            path = request.args[0]
            dirent = std::directory_entry(path)

            if not dirent.exists() or not dirent.is_regular_file():
                return -  // File does not exist

            if not dirent.is_accessible_by(client.username):
                return -  // File not accessible by user

            client.pending_request = request
            return +  // Success
            */

        case request_type::send:
            YONAA_DEBUG("Handling SEND request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 0
            // - there is a pending retr request

            /*
            if request.args.size() != 0
                return -

            if not client.logged_in:
                return -  // User is not logged in

            if not client.pending_request and client.pending_request.type !=
            request_type::retr: return -  // No pending retr request

            ... send file data ...

            client.pending_request = std::nullopt
            return +  // Success
            */

        case request_type::stop:
            YONAA_DEBUG("Handling STOP request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 0
            // - there is a pending retr request

            /*
            if request.args.size() != 0
                return -

            if not client.logged_in:
                return -  // User is not logged in

            if not client.pending_request and client.pending_request.type !=
            request_type::retr: return -  // No pending retr request

            client.pending_request = std::nullopt
            return +  // Success
            */

        default:
            assert(0);
    }
}

void server::handle_stor(yonaa::client_id client_id, const request &request) {
    (void)client_id;
    (void)request;

    switch (request.type) {
        case request_type::stor:
            YONAA_DEBUG("Handling STOR request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 2
            // - args[0] = "NEW", "OLD", or "APP"
            // - args[1] exists
            // - args[1] is accessible by the user

        case request_type::size:
            YONAA_DEBUG("Handling SIZE request");

            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 1
            // - there is a pending stor request

        default:
            assert(0);
    }
}

}  // namespace sftp
