/*For download & reading files*/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include "thread_pool.hpp"

/*For create/delete files*/
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif
/*For resize image*/
#ifdef WITH_OPENCV
#include <opencv2/opencv.hpp>
#endif

/*
 argv[1]: input csv file
 argv[2]: output folder
 "optional"
 argv[3]: scale value
 */
void showhelp(){
    printf("----------------Usage----------------\n");
    printf("Passing at leaset two arguments to start:\n \
    ./csvdownloader arg1 arg2 arg3\n");
    printf("arg1: input csv path.\n");
    printf("arg2: Output image folder.\n");
    printf("arg3: scale value. (Optional)\n");
    printf("*Scaling image requires OpenCV\n");
    printf("-------------------------------------\n");
    exit(0);
}
//!!!!: READ CSV
void create_folder(std::string name){
    #ifdef WIN32
        _mkdir(name);
    #else
        mkdir(name.c_str(), 0755);
    #endif
}
/** Return 1 if found, 0 if not found. */
int stringstok(std::string input, std::string& first, std::string& second, std::string stok){
    size_t condition;
    condition = input.find(stok);
//    try {
//        condition = input.find(stok);
//        if(condition == std::string::npos) throw stok;
//    }
//    catch (std::string str) {
//        printf("No specified \"%s\" found.\n", str.c_str());
//    }
    if(condition != std::string::npos){
        first.assign(input.begin(), input.begin()+condition);
        second.assign(input.begin()+condition+1, input.end());
        return 1;
    }
    return 0;
}
void erase_chracter(std::string& input, std::string charact){
    size_t a = input.find(charact);
    do {
        a = input.find(charact);
        if (a != std::string::npos){
            input.erase(a, charact.size());
        }
    } while (a != std::string::npos);
}
std::vector<std::vector<std::string> > loadcsv(const std::string& path){
    std::vector<std::vector<std::string> > buf;
    std::ifstream file(path.c_str());
    std::string line;
    while(getline(file, line)){
        std::vector<std::string> subbuf;
//        printf("%s\n",line.c_str());

        std::string first="", second="";
        do {
            first = ""; second = "";
            if(stringstok(line, first, second, ",")){
                erase_chracter(first,"\"");
                erase_chracter(second,"\"");
                subbuf.push_back(first);
                line = second;
            }
        } while (second != "");
        subbuf.push_back(line);
        buf.push_back(subbuf);
//        for(auto sub:subbuf) printf("%s ", sub.c_str());
//        printf("\n");

    }
    return buf;
}
#ifdef WITH_OPENCV
int scaleimage(const std::string& path, const float& scale){
    cv::Mat image;
    image = cv::imread(path);
    if(image.cols == 0){
        printf("not able to scale image: %s\n",path.c_str());
        return 0;
    }
    cv::resize(image, image, cv::Size(image.cols*scale, image.rows*scale));
    cv::imwrite(path, image);
    return 1;
}
#endif
//!!!!: CURL FUNCTIONS
/** For webpage information */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
/** For image or file */
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}
void curldownload(std::vector<std::string>& input, const std::string& folder_path, float compress = 1.0f, std::string format = ".jpg"){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string& filename = input[0];
        std::string& url = input[1];
        
        std::string filepath = folder_path+filename+format;
        char* filepath_char = new char [filepath.size() + 1];
        std::strcpy(filepath_char, filepath.c_str());
        
        char* url_char = new char [url.size()+1];
        std::strcpy(url_char, url.c_str());
        FILE *fp;
        fp = fopen(filepath_char, "wb");
        if( fp == NULL ) std::cout << "File cannot be opened\n";
        
        curl_easy_setopt(curl, CURLOPT_URL, url_char);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        res = curl_easy_perform(curl);
        if( res )std::cout << "Cannot grab the image!\n";
        
        curl_easy_cleanup(curl);
        fclose(fp);
        
        delete [] url_char;
        delete [] filepath_char;
        
#ifdef WITH_OPENCV
        if(compress != 1.0f) scaleimage(filepath, compress);
#endif
    }
}
int main(int argc, char* argv[]){
    if(strncmp(argv[1], "-h", 2)==0||strncmp(argv[2], "-h", 3)==0||argc<3)showhelp();
    std::string input_path = argv[1];
    std::string output_path = argv[2];
    float scale = 1.0;
    if(argc == 4) {
    #ifndef WITH_OPENCV
        printf("Warning: OpenCV not found. Scaling will not be performed\n");;
    #endif
        scale = std::atof(argv[3]);
    }
    printf("Input file:      %s\n",input_path.c_str());
    printf("Output file:     %s\n",output_path.c_str());
    printf("Scale factor:    %f\n",scale);
    printf("Runnign threads: %d\n", std::thread::hardware_concurrency());
    
    TaskThreadPool pool(std::thread::hardware_concurrency());
    create_folder(output_path);
    std::vector<std::vector<std::string> > data = loadcsv(input_path);
    for(size_t i=1;i<data.size();++i){
        pool.runTaskWithID(std::bind(curldownload, data[i], output_path, scale, ".jpg"));
    }
    pool.waitWorkComplete();
    printf("Done!.\n");
    return 0;
}


