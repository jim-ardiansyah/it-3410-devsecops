#include <iostream>
#include <string>

int main() {
    std::string appName = "CI/CD Pipeline Demo";
    int buildNumber = 1;

    std::cout << "Hello from " << appName << "!" << std::endl;
    std::cout << "Build number: " << buildNumber << std::endl;
    std::cout << "If you can read this, CodeBuild compiled the code successfully." << std::endl;

    return 0;
}
