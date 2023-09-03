#include "sftp_server.hpp"

sftp_server::sftp_server(uint16_t port) : server_(port) {
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
    handlers_ = {
        // Insert a NOP for NONE
        {sftp_request_type::NONE,
         [](yonaa::client_id id, const sftp_request &request) {
             (void)id;
             (void)request;
         }},

        {sftp_request_type::USER, std::bind(&sftp_server::handle_USER, this, _1, _2)},
        {sftp_request_type::ACCT, std::bind(&sftp_server::handle_ACCT, this, _1, _2)},
        {sftp_request_type::PASS, std::bind(&sftp_server::handle_PASS, this, _1, _2)},
        {sftp_request_type::TYPE, std::bind(&sftp_server::handle_TYPE, this, _1, _2)},
        {sftp_request_type::LIST, std::bind(&sftp_server::handle_LIST, this, _1, _2)},
        {sftp_request_type::CDIR, std::bind(&sftp_server::handle_CDIR, this, _1, _2)},
        {sftp_request_type::KILL, std::bind(&sftp_server::handle_KILL, this, _1, _2)},
        {sftp_request_type::NAME, std::bind(&sftp_server::handle_NAME, this, _1, _2)},
        {sftp_request_type::TOBE, std::bind(&sftp_server::handle_NAME, this, _1, _2)},
        {sftp_request_type::DONE, std::bind(&sftp_server::handle_DONE, this, _1, _2)},
        {sftp_request_type::RETR, std::bind(&sftp_server::handle_RETR, this, _1, _2)},
        {sftp_request_type::SEND, std::bind(&sftp_server::handle_RETR, this, _1, _2)},
        {sftp_request_type::STOP, std::bind(&sftp_server::handle_RETR, this, _1, _2)},
        {sftp_request_type::STOR, std::bind(&sftp_server::handle_STOR, this, _1, _2)},
        {sftp_request_type::SIZE, std::bind(&sftp_server::handle_STOR, this, _1, _2)},
    };
}

void sftp_server::handle_client_connect(yonaa::client_id client_id) {
    (void)client_id;
}

void sftp_server::handle_client_disconnect(yonaa::client_id client_id) {
    (void)client_id;
}

void sftp_server::handle_request(yonaa::client_id client_id, const yonaa::buffer &data) {
    // Transform the raw data into a sensible request
    auto raw_bytes                      = std::string_view(data.data(), data.size());
    std::optional<sftp_request> request = sftp_request::from_bytes(raw_bytes);

    // If the data can't be made into a valid request, assume that this is a client that we don't
    // want to communicate with
    if (!request) {
        server_.remove_client(client_id);
        return;
    }

    // Get the handler for this type of request
    request_handler handler_function = handlers_[request->type];

    // If there is a pending request, use that request type instead
    sftp_session_info session = sessions_[client_id];
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

void sftp_server::handle_USER(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

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

void sftp_server::handle_ACCT(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

    /*
    if request.args.size() != 1
        return -

    return + // no effect, feature not implemented
    */
}

void sftp_server::handle_PASS(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

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

void sftp_server::handle_TYPE(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

    /*
    if not client.logged_in:
        return -  // User not logged in

    return +  // No effect; only binary mode is supported
    */
}

void sftp_server::handle_LIST(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

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

void sftp_server::handle_CDIR(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

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

void sftp_server::handle_KILL(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

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

void sftp_server::handle_NAME(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

    switch (request.type) {
        case sftp_request_type::NAME:
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

            if not dirent.exists() or not dirent.is_directory():
                return -  // Invalid directory

            if not dirent.is_accessible_by(client.username):
                return -  // Directory not accessible by user

            client.pending_request = request
            return +  // Success
            */

        case sftp_request_type::TOBE:
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
            sftp_request_type::NAME: return -  // No pending rename request

            std::filesystem::rename(client.pending_request.args[0], request.args[0])
            client.pending_request = std::nullopt
            return +  // Success
            */

        default:
            assert(0);
    }
}

void sftp_server::handle_DONE(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

    // Success pre-conditions:
    // - arg count = 0

    /*
    if request.args.size() != 0
        return -

    return +  // Success
    */
}

void sftp_server::handle_RETR(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

    switch (request.type) {
        case sftp_request_type::RETR:
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

            if not dirent.exists() or not dirent.is_regular_file():
                return -  // File does not exist

            if not dirent.is_accessible_by(client.username):
                return -  // File not accessible by user

            client.pending_request = request
            return +  // Success
            */

        case sftp_request_type::SEND:
            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 0
            // - there is a pending RETR request

            /*
            if request.args.size() != 0
                return -

            if not client.logged_in:
                return -  // User is not logged in

            if not client.pending_request and client.pending_request.type != sftp_request_type::RETR:
                return -  // No pending RETR request

            ... send file data ...

            client.pending_request = std::nullopt
            return +  // Success
            */

        case sftp_request_type::STOP:
            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 0
            // - there is a pending RETR request

            /*
            if request.args.size() != 0
                return -

            if not client.logged_in:
                return -  // User is not logged in
            
            if not client.pending_request and client.pending_request.type != sftp_request_type::RETR:
                return -  // No pending RETR request

            client.pending_request = std::nullopt
            return +  // Success
            */

        default:
            assert(0);
    }
}

void sftp_server::handle_STOR(yonaa::client_id client_id, const sftp_request &request) {
    (void)client_id;
    (void)request;

    switch (request.type) {
        case sftp_request_type::STOR:
            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 2
            // - args[0] = "NEW", "OLD", or "APP"
            // - args[1] exists
            // - args[1] is accessible by the user

        case sftp_request_type::SIZE:
            // Success pre-conditions:
            // - the client is logged in
            // - arg count = 1
            // - there is a pending STOR request

        default:
            assert(0);
    }
}
