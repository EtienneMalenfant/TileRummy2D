#include <io/i_input_stream.h>
#include <fstream>

class FileInput : public IInputStream {
private:
    std::ifstream file;
public:
    FileInput(const std::string& path);
    ~FileInput() override;

    std::string readNext() override;
    bool hasNext() override;
};
