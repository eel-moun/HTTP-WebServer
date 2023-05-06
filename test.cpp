#include "./headers/parseConfig.hpp"

int main(){
    
    ConfigFile file;

    file.setServer();
    file.getServer(0)->setListen("1900");
    std::cout<< "===>> " << file.getServer(0)->getListen() << std::endl;

    // cout << file.getSize() << endl;
    // cout <<"--------------------------------------" << endl;
    
    // file.setServer();
    // cout << file.getSize() << endl;

    // cout <<"--------------------------------------" << endl;

    // file.getServer(0).setListen("100");
    // cout << file.getServer(0).getListen() << endl;

    // cout <<"--------------------------------------" << endl;
}