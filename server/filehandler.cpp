#include "filehandler.h"

#include <ctime>
#include <iostream>

#include <algorithm>
#include <sstream>
#include <string.h>

namespace bon_server {

FileHandler::FileHandler(int id)
{
    id_ = id;

    std::time_t ts = std::time(nullptr);
    file_name_ = std::to_string(ts) + "-" + std::to_string(id);
    std::cout << "New: " << file_name_ << std::endl;
}

FileHandler::~FileHandler()
{
    file_stream_.close();
}

bool FileHandler::AddNewInfo(const Record &rec)
{
    if(!file_stream_.is_open())
        file_stream_.open(file_name_ + ".csv", std::ofstream::out);

    file_stream_ << rec.e1.i << ',';
    file_stream_ << rec.e2.f << ',';
    file_stream_ << rec.e3.i << std::endl;
}

bool FileHandler::Finalize()
{
    file_stream_.close();

    std::ifstream in_stream(file_name_ + ".csv");
    if(in_stream.fail())
    {
        std::cerr << "Can't open file" << std::endl;
        return false;
    }

    std::vector<Record> sort_container;

    std::string line;
    while(std::getline(in_stream, line)) //read line from file
    {
        std::string elem;
        std::istringstream lineStream(line);
        Record rec;

        // read elems from line
        std::getline(lineStream, elem, ',');
        rec.e1.i = std::stoi(elem);
        std::getline(lineStream, elem, ',');
        rec.e2.f = std::stof(elem);
        std::getline(lineStream, elem, ',');
        rec.e3.i = std::stoi(elem);

        sort_container.push_back(rec);
    }

    // Write sorted files
    std::sort(sort_container.begin(), sort_container.end(), [](const Record &rec1, const Record &rec2)->bool
    {
        return rec1.e1.i < rec2.e1.i;
    });
    WriteToFile(sort_container, "_sort1");

    std::sort(sort_container.begin(), sort_container.end(), [](const Record &rec1, const Record &rec2)->bool
    {
        return rec1.e2.f > rec2.e2.f;
    });
    WriteToFile(sort_container, "_sort2");

    std::sort(sort_container.begin(), sort_container.end(), [](const Record &rec1, const Record &rec2)->bool
    {
        return rec1.e3.i < rec2.e3.i;
    });
    WriteToFile(sort_container, "_sort3");

    std::cout << "Finished: " << file_name_ << std::endl;

    return true;
}

void FileHandler::WriteToFile(std::vector<Record> sort_container, const std::string &postfix)
{    
    std::string fn = file_name_ + postfix + ".csv";
    std::ofstream out;
    out.open(fn, std::ofstream::out);
    if(out.fail())
    {
        std::cerr << "Can't open file:" << fn << std::endl;
        return;
    }

    for(const auto &rec : sort_container)
    {
        out << rec.e1.i << ',';
        out << rec.e2.f << ',';
        out << rec.e3.i << std::endl;
    }
    out.close();
}

} //NS
