class UDP
{
public:
    UDP();
    ~UDP();
    void initUDP();
    void UDP_COM(const char *message);

private:
    /// @brief Socket descriptor for the UDP connection
    int socket_desc_;

    /// @brief Hardcoded IP address of UDP server
    const char *IP;
};