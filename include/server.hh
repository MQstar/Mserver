namespace server{
class Server
{
    public:
    Server();
    ~Server();
    void start();
    void close();
    private:
    bool isListeningM;
    int socketM;
    bool setNonblock();
};
}
