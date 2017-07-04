#include <iostream>
#include <string>
#include <time.h>
#include <assert.h>
#include "binary_file_parser.h"
#include "snapshot.h"
#include "output.h"

std::ostream& operator<< (std::ostream& ost, const Message& m) {
	const std::string pre = " | ";
	ost << pre << "Time: " << m.Time();
	ost << pre << "Type: " << m.Type();
	ost << pre << "ind: " << m.BSInd();
	ost << pre << "Ref: " << m.Ref();
	ost << pre << "NewRef: " << m.NewRef();
	ost << "\n";
	return ost;
}

void Run(std::initializer_list<std::string> stocks, std::initializer_list<Label> labels, double until, bool w_dup = true) {
	const std::string input_folder("..\\..\\NasdaqData\\");
	const std::string output_folder("..\\outputs\\");
	const std::string file_name("S080613-v41");
    BinaryFileParser parser(input_folder + file_name + ".txt");
	Snapshot snapshot(stocks);
	std::string prefix = output_folder;
	std::unordered_map<std::string, std::vector<std::unique_ptr<TSWriter>>> writers;
	for (auto label : labels) 
		for (auto stock : stocks) 
			writers[stock].emplace_back(Output::NewTSWriter(stock, label, prefix + stock + "_" + Output::LabelStr(label) + ".txt", w_dup));
	while (!parser.IsEnd() && parser.GetSecond() <= until) {
		std::unique_ptr<Message> pmessage = parser.ReadMessage();
		if (pmessage) {
			std::string stock = snapshot.ProcessMessage(pmessage.get());
			if (stock != "")
				for (auto &writer : writers[stock]) Output::Update(writer.get(), &snapshot);
		}
	}
}

int main() {
	clock_t t1 = clock();

	double until = 12 * 3600;
	Run({ "SPY", "SDS" }, { MAX_BID, MIN_ASK, MAX_BID_VOL, MIN_ASK_VOL },until,false);

	clock_t t2 = clock();
	std::cout << ((double)t2 - (double)t1) / CLOCKS_PER_SEC << " seconds\n";
	return 0;
}