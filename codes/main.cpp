#include <iostream>
#include <string>
#include <time.h>
#include <assert.h>
#include "binary_file_parser.h"
#include "snapshot.h"
#include "output.h"


void Run(const std::vector<std::string>& stocks, const std::vector<Label>& labels, double until = 24*3600) {
	const std::string input_folder("..\\..\\NasdaqData\\");
	const std::string output_folder("..\\..\\LimitOrderBookOutputs\\");
	const std::string file_name("S041813-v41");
    BinaryFileParser parser(input_folder + file_name + ".txt");
	Snapshot snapshot(stocks);
	std::string prefix = output_folder + file_name;
	std::unordered_map<std::string, std::vector<std::unique_ptr<TSWriter>>> writers;
	for (size_t i = 0; i < labels.size(); i++) 
		for (auto stock : stocks) 
			writers[stock].emplace_back(Output::NewTSWriter(stock, labels[i], 
														   prefix + "_" + stock + "_" + Output::LabelStr(labels[i]) + ".txt"));
	while (!parser.IsEnd() && parser.GetSecond() <= until) {
		std::unique_ptr<Message> pmessage = parser.ReadMessage();
		if (pmessage) {
			auto p = snapshot.ProcessMessage(pmessage.get());
			if (p.first) 
				for (auto &writer : writers[p.second]) Output::Update(writer.get(), &snapshot);
		}
	}
}

int main() {
	clock_t t1 = clock();

	const std::vector<std::string> stocks{ "SPY", "SDS" };
	const std::vector<Label> labels{ MAX_BID, MIN_ASK, MAX_BID_VOL, MIN_ASK_VOL };
	double until = 12 * 3600;
	Run(stocks,labels,until);

	clock_t t2 = clock();
	std::cout << ((double)t2 - (double)t1) / CLOCKS_PER_SEC << " seconds\n";
	return 0;
}