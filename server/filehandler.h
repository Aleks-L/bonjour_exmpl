#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <fstream>

namespace bon_server {

class FileHandler
{
public:
    union RecordElem
    {
        int32_t i;
        float   f;

        RecordElem() {i = 0;}
        RecordElem(int val) {i = val;}
        RecordElem(float val) {f = val;}
        RecordElem(double val) {f = static_cast<float>(val);}
    };

    struct Record{
        RecordElem e1;
        RecordElem e2;
        RecordElem e3;
    };

    explicit FileHandler(int id);
    ~FileHandler();

    inline int id() const {return id_;}

    bool AddNewInfo(const Record &rec);
    bool Finalize();

private:
    int id_;
    std::string file_name_;
    std::ofstream file_stream_;

    void WriteToFile(std::vector<Record> vec, const std::string &postfix);
};

} //NS

#endif // FILEHANDLER_H
