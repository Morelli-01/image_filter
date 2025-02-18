#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include <boost/program_options.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/text/ocr.hpp>

#include "algorithm"
#include "cstdlib"
#include "filesystem"
#include "format"
#include "omp.h"
#include "ranges"
#include "string"
#include "unistd.h"
#include "vector"

using namespace std;
namespace fs = std::filesystem;
namespace po = boost::program_options;

string extract_text_from_image(std::string image_path) {
    cv::Mat image;
    if (image_path.contains("avi")) {
        cv::VideoCapture cap(image_path);
        if (!cap.isOpened() or cap.get(cv::CAP_PROP_FRAME_COUNT) <= 0) {
            std::cerr << "Error: Could not open video file.\n";
            return "";
        }
        cap >> image;  // extract only first frame
    } else
        image = cv::imread(image_path, cv::IMREAD_COLOR);
    cv::Ptr<cv::text::OCRTesseract> ocr = cv::text::OCRTesseract::create(NULL, "ita");
    string extracted_text;
    ocr->run(image, extracted_text);
    return extracted_text;
}

int main(int argc, char **argv) {
    po::options_description desc("Options");
    desc.add_options()("help,h", "Show Help")("input_path,i", po::value<std::string>()->required(), "Specify input path")("output_path,o", po::value<std::string>()->required(), "Specify output path")("ext,e", po::value<std::string>()->default_value(".bmp"), "Specify img ext to filter")("text,t", po::value<std::string>()->required(), "Specify text to look for in images");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    po::notify(vm);

    const string root_path = vm["input_path"].as<string>();
    const string out_path = vm["output_path"].as<string>();
    const string search_text = vm["text"].as<string>();
    const string ext = vm["ext"].as<string>();

    if (!fs::is_directory(root_path)) {
        cerr << "The root_path is not valid\n";
        return EXIT_FAILURE;
    }

    if (!fs::is_directory(out_path))
        fs::create_directory(out_path);

    vector<string> img_to_check;

    for (const auto &entry : fs::recursive_directory_iterator(root_path)) {
        if (!fs::is_directory(entry) && entry.path().string().find(ext) != std::string::npos) {
            img_to_check.push_back(entry.path().string());
        }
    }
    cout << format("There are a total of {} files to check with extension \"{}\"", img_to_check.size(), ext) << endl;

#pragma omp parallel for
    for (size_t i = 0; i < img_to_check.size(); i++) {
        string extracted_text = extract_text_from_image(img_to_check[i]);
        if (extracted_text.find(search_text) != std::string::npos) {
            fs::path out_file_name = fs::path(out_path);
            out_file_name.append(fs::path(img_to_check[i]).filename().string());
            string cmd =
                "cp '" + img_to_check[i] + "' '" + out_file_name.c_str() + "'";
            int ret_code = system(cmd.c_str());
            if (ret_code == 0)
                cout << img_to_check[i] << endl;
            else {
                perror(cmd.c_str());
                perror((string("Errore copia ") + img_to_check[i]).c_str());
            }
        }
    }

    return EXIT_SUCCESS;
}
