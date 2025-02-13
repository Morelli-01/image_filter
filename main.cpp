#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "string"
#include "filesystem"
#include "omp.h"
#include "vector"
#include "ranges"
#include "algorithm"
#include "cstdlib"
#include <opencv2/opencv.hpp>
#include <opencv2/text/ocr.hpp>
#include "unistd.h"
#include <boost/program_options.hpp>

using namespace std;
namespace fs = std::filesystem;
namespace po = boost::program_options;

string extract_image_cv(const std::string &image_path)
{
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR); 
    cv::Ptr<cv::text::OCRTesseract> ocr = cv::text::OCRTesseract::create(NULL, "ita");
    string extracted_text;
    ocr->run(image, extracted_text);
    return extracted_text;
}

int main(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()("help,h", "Show Help")("input_path,i", po::value<std::string>()->required(), "Specify input path")("output_path,o", po::value<std::string>()->required(), "Specify output path")("ext,e", po::value<std::string>()->default_value(".bmp"), "Specify img ext to filter")("text,t", po::value<std::string>()->required(), "Specify text to look for in images");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    po::notify(vm);


    const string root_path = vm["input_path"].as<string>();
    const string out_path = vm["output_path"].as<string>();
    const string search_text = vm["text"].as<string>();
    const string ext = vm["ext"].as<string>();

    if (!fs::is_directory(root_path))
    {
        cerr << "The root_path is not valid\n";
        return EXIT_FAILURE;
    }

    if (!fs::is_directory(out_path))
        fs::create_directory(out_path);

    vector<string> img_to_check;

    for (const auto &entry : fs::recursive_directory_iterator(root_path))
    {
        if (!fs::is_directory(entry) && entry.path().string().find(ext) != std::string::npos)
        {
            img_to_check.push_back(entry.path().string());
        }
    }

#pragma omp parallel for
    for (size_t i = 0; i < img_to_check.size(); i++)
    {
        string extracted_text = extract_image_cv(img_to_check[i]);
        if (extracted_text.find(search_text) != std::string::npos)
        {
            string cmd =
                "cp '" + img_to_check[i] + "' '" + out_path + fs::path(img_to_check[i]).filename().string() +
                "'";
            int ret_code = system(cmd.c_str());
            if (ret_code == 0)
                cout << img_to_check[i] << endl;
            else
            {
                perror(cmd.c_str());
                perror((string("Errore copia ") + img_to_check[i]).c_str());
            }
        }
    }

    return EXIT_SUCCESS;
}
