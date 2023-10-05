#include <fstream>
#include <vector>
#include <utility>
#include <string>

typedef std::vector<double> dVector;
typedef std::vector<std::vector<double>> ddVector;
typedef std::vector<std::vector<std::vector<double>>> dddVector;
typedef std::vector<std::pair<dVector, int>> Data;

Data readTrainingData(){
    return readData("data/train");
}

Data readTestData(){
    return readData("data/t10k");
}

Data readData(std::string pathname){
    Data data;
    std::string imagesPath = pathname + "-labels-idx1-ubyte";
    std::string labelsPath = pathname + "-images-idx3-ubyte";
    int magicNumber, numberOfItems, nRows, nCols;
    std::ifstream fin; fin.open(imagesPath);
    fin.read(&magicNumber, 4);
    fin.read(&nRows, 4);
    fin.read(&nCols, 4);
    ddVector intensity(numberOfItems, dVector(nRows * nCols));
    for(int i = 0; i < numberOfItems; i++){
        for(int j = 0; j < nRows * nCols; j++){
            intensity[i][j] = (double)(fin.get()) / 255;
        }
    }

    fin.close();

    fin.open(labelsPath);

    magicNumber = fin.get();


}
