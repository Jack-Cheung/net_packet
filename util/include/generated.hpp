#define HEADER_PKGLEN_VAR_NAME packet_len 

#define HEADER_PROPERTY_DECL(varname, bytes) uint8_t varname [ bytes ];\

#define HEADER_PKGLEN_PROPERTY_DECL(varname) uint64_t varname;


#define HEADER_PROPERTY_LIST \
HEADER_PROPERTY_DECL(version, 1)\
HEADER_PROPERTY_DECL(operation, 1)\
HEADER_PROPERTY_DECL(unfixed_size, 1)\
HEADER_PKGLEN_PROPERTY_DECL(HEADER_PKGLEN_VAR_NAME)\
HEADER_PROPERTY_DECL(open_session, 1)

#define VALUE_PARAM_LEN_LEN  8 //byte
#define VALUE_HEADER_LEN 12


#define CODE_PIECE_HEADER_OSTREAM \
    os.write((const char*)&header.version, sizeof(header.version))\
    .write((const char*)&header.operation, sizeof(header.operation))\
    .write((const char*)&header.unfixed_size, sizeof(header.unfixed_size))\
    .write((const char*)&header.packet_len, sizeof(header.packet_len))\
    .write((const char*)&header.open_session, sizeof(header.open_session));

#define CODE_PIECE_HEADER_ISTREAM \
    is.read((char*)&header.version, sizeof(header.version))\
    .read((char*)&header.operation, sizeof(header.operation))\
    .read((char*)&header.unfixed_size, sizeof(header.unfixed_size))\
    .read((char*)&header.packet_len, sizeof(header.packet_len))\
    .read((char*)&header.open_session, sizeof(header.open_session));

#define CODE_PIECE_HEADER_CTOR \
    ::memset(version, 0, sizeof(version));\
    ::memset(operation, 0, sizeof(operation));\
    ::memset(unfixed_size, 0, sizeof(unfixed_size));\
    packet_len = 0;\
    ::memset(open_session, 0, sizeof(open_session));

#define CODE_PIECE_HEADER_PRINTER \
    os << "version\t\t";\
    const char* buf = get_hex_string(version , sizeof(version), buf_size);\
    os << buf;\
    delete[] buf;\
    os << endl;\
    \
    os << "operation code\t\t";\
    buf =  get_hex_string(operation , sizeof(operation), buf_size);\
    os << buf;\
    delete[] buf;\
    os << endl;\
    \
    os << "unfixed size\t\t";\
    buf =  get_hex_string(unfixed_size , sizeof(unfixed_size), buf_size);\
    os << buf;\
    delete[] buf;\
    os << endl;\
    \
    os << "packet length\t\t";\
    os << to_string(packet_len) << endl;\
    \
    os << "if open session\t\t";\
    buf =  get_hex_string(open_session , sizeof(open_session), buf_size);\
    os << buf;\
    delete[] buf;\
    os << endl;
