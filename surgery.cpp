#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;


struct Surgery {
    string startDate;
    string endDate;
    string substationName;
    string hospitalName;
    string teamName;
    string surgeryType;
    int surgeryTime;
    int difficultyLevel;
};

vector<Surgery> surgeries;

void read_csv_file(string HospitalDatabase_V1.1) {
    ifstream file(HospitalDatabase_V1.1);
    if (!file.is_open()) {
        cout << "Error opening file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        string startDate, endDate, substationName, hospitalName, teamName, surgeryType;
        int surgeryTime, difficultyLevel;

        getline(ss, startDate, ',');
        getline(ss, endDate, ',');
        getline(ss, substationName, ',');
        getline(ss, hospitalName, ',');
        getline(ss, teamName, ',');
        getline(ss, surgeryType, ',');
        ss >> surgeryTime;
        ss.ignore();
        ss >> difficultyLevel;

        surgeries.push_back({ startDate, endDate, substationName, hospitalName, teamName, surgeryType, surgeryTime, difficultyLevel });
    }

    file.close();
}

void add_team() {
    string hospitalName, teamName;
    cout << "Enter hospital name: ";
    getline(cin, hospitalName);
    cout << "Enter team name: ";
    getline(cin, teamName);
    // check if team already exists
    auto it = find_if(surgeries.begin(), surgeries.end(), [&](const Surgery& s) {
        return s.hospitalName == hospitalName && s.teamName == teamName;
    });
    if (it == surgeries.end()) {
        surgeries.push_back({ "", "", "", hospitalName, teamName, "", 0, 0 });
        cout << "Team added successfully." << endl;
    } 
    else {
        cout << "Team already exists." << endl;
    }
}

void remove_team() {
    string hospitalName, teamName;
    cout << "Enter hospital name: ";
    getline(cin, hospitalName);
    cout << "Enter team name: ";
    getline(cin, teamName);
    // remove all surgeries performed by the team
    surgeries.erase(remove_if(surgeries.begin(), surgeries.end(), [&](const Surgery& s) {
        return s.hospitalName == hospitalName && s.teamName == teamName;
    }), surgeries.end());
    // remove team
    auto it = remove_if(surgeries.begin(), surgeries.end(), [&](const Surgery& s) {
        return s.hospitalName == hospitalName && s.teamName == teamName && s.surgeryTime == 0 && s.difficultyLevel == 0;
    });
    if (it != surgeries.end()) {
        surgeries.erase(it);
        cout << "Team removed successfully." << endl;
    } 
    else {
        cout << "Team does not exist." << endl;
    }
}

void add_surgeon() {
    string hospitalName, teamName, surgeonName;
    cout << "Enter hospital name: ";
    getline(cin, hospitalName);
    cout << "Enter team name: ";
    getline(cin, teamName);
    cout << "Enter surgeon name: ";
    getline(cin, surgeonName);
    // check if team exists
    auto it = find_if(surgeries.begin(), surgeries.end(), [&](const Surgery& s) {
        return s.hospitalName == hospitalName && s.teamName == teamName;
    });
    if (it != surgeries.end()) {
	// add surgeon to team
	it->substationName = surgeonName;
	cout << "Surgeon added successfully." << endl;
	} 
    else {
	     cout << "Team does not exist." << endl;
	}
}

void remove_surgeon() {
	string hospitalName, teamName;
	cout << "Enter hospital name: ";
	getline(cin, hospitalName);
	cout << "Enter team name: ";
	getline(cin, teamName);
	// check if team exists
	auto it = find_if(surgeries.begin(), surgeries.end(), [&](const Surgery& s) {
	return s.hospitalName == hospitalName && s.teamName == teamName;
	});
	if (it != surgeries.end()) {
	// remove surgeon from team
	it->substationName = "";
	cout << "Surgeon removed successfully." << endl;
	} 
    else {
	     cout << "Team does not exist." << endl;
    }
}

void list_teams() {
	vector<string> teams;
	for (const auto& s : surgeries) {
	    if (s.surgeryTime == 0 && s.difficultyLevel == 0) { // only list teams, not surgeries
	       string team = s.hospitalName + " - " + s.teamName;
	       if (find(teams.begin(), teams.end(), team) == teams.end()) {
	          teams.push_back(team);
           }
        }
	}
	cout << "List of teams:" << endl;
	for (const auto& team : teams) {
	    cout << "- " << team << endl;
	}
}

void list_surgeries() {
	vector<string> surgeryTypes;
	for (const auto& s : surgeries) {
	    if (s.surgeryTime > 0 && s.difficultyLevel > 0) { // only list surgeries, not teams
	       if (find(surgeryTypes.begin(), surgeryTypes.end(), s.surgeryType) == surgeryTypes.end()) {
	          surgeryTypes.push_back(s.surgeryType);
           }
        }
	}
	cout << "List of surgeries:" << endl;
	for (const auto& surgeryType : surgeryTypes) {
	    cout << "- " << surgeryType << endl;
     }
}

void calculate_average_difficulty() {
	string hospitalName, teamName;
	cout << "Enter hospital name: ";
	getline(cin, hospitalName);
	cout << "Enter team name: ";
	getline(cin, teamName);
	// get all surgeries performed by the team
	vector<const Surgery*> teamSurgeries;
	for (const auto& s : surgeries) {
	    if (s.hospitalName == hospitalName && s.teamName == teamName && s.surgeryTime > 0 && s.difficultyLevel > 0) {
	       teamSurgeries.push_back(&s);
        }
	}
	if (teamSurgeries.empty()) {
	   cout << "Team does not exist or has not performed any surgeries." << endl;
	} 
    else {
	// calculate average difficulty level
	   double sum = accumulate(teamSurgeries.begin(), teamSurgeries.end(), 0.0, [](double a, const Surgery* b) {
	          return a + b->difficultyLevel;
       });
	   double avg = sum / teamSurgeries.size();
	   cout << "Average difficulty level of surgeries performed by " << teamName << " at " << hospitalName << " is " << avg << "." << endl;
	}
}
void view_surgery_statistics() {
    int totalSurgeries = surgeries.size();
    int successfulSurgeries = 0;
    int failedSurgeries = 0;

    for (const auto& surgery : surgeries) {
        if (surgery.endDate != "") {
            successfulSurgeries++;
        } else {
            failedSurgeries++;
        }
    }

    double successRate = (static_cast<double>(successfulSurgeries) / totalSurgeries) * 100;

    cout << "Surgery Statistics:" << endl;
    cout << "Total Surgeries: " << totalSurgeries << endl;
    cout << "Successful Surgeries: " << successfulSurgeries << endl;
    cout << "Failed Surgeries: " << failedSurgeries << endl;
    cout << "Success Rate: " << successRate << "%" << endl;
}

void view_surgery_schedule() {
    cout << "Surgery Schedule:" << endl;
    for (const auto& surgery : surgeries) {
        cout << "Hospital: " << surgery.hospitalName << endl;
        cout << "Team: " << surgery.teamName << endl;
        cout << "Surgery Type: " << surgery.surgeryType << endl;
        cout << "Start Date: " << surgery.startDate << endl;
        cout << "End Date: " << surgery.endDate << endl;
        cout << endl;
    }
}

void view_team_schedule() {
    cout << "Team Schedule:" << endl;
    for (const auto& surgery : surgeries) {
        cout << "Team: " << surgery.teamName << endl;
        cout << "Hospital: " << surgery.hospitalName << endl;
        cout << "Surgery Type: " << surgery.surgeryType << endl;
        cout << "Start Date: " << surgery.startDate << endl;
        cout << "End Date: " << surgery.endDate << endl;
        cout << endl;
    }
}

void view_surgeon_schedule() {
    cout << "Surgeon Schedule:" << endl;
    for (const auto& surgery : surgeries) {
        cout << "Team: " << surgery.teamName << endl;
        cout << "Hospital: " << surgery.hospitalName << endl;
        cout << "Surgeon(s): ";
        for (const auto& surgeon : surgery.teamName) {
            cout << surgeon << ", ";
        }
        cout << endl;
        cout << "Surgery Type: " << surgery.surgeryType << endl;
        cout << "Start Date: " << surgery.startDate << endl;
        cout << "End Date: " << surgery.endDate << endl;
        cout << endl;
    }
}

void optimize_capacity(double availableCapacity) {
    // Step 1: Estimate the number of hours required for different surgeries
    vector<double> estimatedHours(surgeries.size(), 0.0);
    for (int i = 0; i < surgeries.size(); i++) {
        estimatedHours[i] = surgeries[i].surgeryTime;  // Use average surgery time as an initial estimate
    }

    // Step 2: Estimate likely surgeries during power outage based on difficulty level
    vector<string> likelySurgeries;
    for (const auto& surgery : surgeries) {
        if (surgery.difficultyLevel >= thresholdValue) {
            likelySurgeries.push_back(surgery.surgeryType);
        }
    }

    // Step 3: Predict the energy required by each team at each hospital
    unordered_map<string, unordered_map<string, double>> energyRequired;  // hospitalName -> teamName -> energy
    for (const auto& surgery : surgeries) {
        double energy = estimatedHours[&surgery - &surgeries[0]] * availableCapacity;
        energyRequired[surgery.hospitalName][surgery.teamName] += energy;
    }

    // Step 4: Distribute the available capacity to maximize outcome
    for (auto& hospital : energyRequired) {
        double hospitalCapacity = availableCapacity;
        for (auto& team : hospital.second) {
            if (team.second > hospitalCapacity) {
                team.second = hospitalCapacity;
            }
            hospitalCapacity -= team.second;
        }
    }

    // Print the distributed capacity for each hospital and team
    for (const auto& hospital : energyRequired) {
        cout << "Hospital: " << hospital.first << endl;
        for (const auto& team : hospital.second) {
            cout << "Team: " << team.first << ", Energy Required: " << team.second << endl;
        }
        cout << endl;
    }
}

struct PowerOutageData {
    string areaCode;
    int totalInterruptions;
    int customersInterrupted;
    int customerHourInterruptions;
};

vector<PowerOutageData> powerOutageData = {
    {"EB", 27, 5769, 16224},
    {"OF", 22, 4413, 12780},
    {"HD", 18, 5990, 12621},
    {"TM", 16, 3436, 11415},
    {"RL", 12, 3529, 11980}
};

double predictPowerOutages() {
    double totalInterruptions = 0.0;
    for (const auto& data : powerOutageData) {
        totalInterruptions += data.totalInterruptions;
    }
    double averageInterruptions = totalInterruptions / powerOutageData.size();
    return averageInterruptions;
}

double estimatePerformanceDuringOutages(double outageTime, double surgeryTime) {
    return (outageTime / surgeryTime) * 100.0;
}

double calculateTotalOutageTime(double outageTime, int numberOfTeams) {
    return outageTime * numberOfTeams;
}

int calculateTotalCustomerHourInterruptions(double outageTime, int numberOfTeams) {
    int totalCustomerHourInterruptions = 0;
    for (const auto& data : powerOutageData) {
        totalCustomerHourInterruptions += data.customerHourInterruptions;
    }
    double averageCustomerHourInterruptions = static_cast<double>(totalCustomerHourInterruptions) / powerOutageData.size();
    return static_cast<int>(averageCustomerHourInterruptions * outageTime * numberOfTeams);
}

int calculate() {
    string filename = "HospitalDatabase_V1.1.csv";
    vector<Surgery> surgeryData = surgeries;;

    double outageTime = predictPowerOutages();
    int numberOfTeams = powerOutageData.size();

    cout << "Predicted power outage duration during the next year: " << outageTime << " hours." << endl;

    for (const auto& surgery : surgeryData) {
        double performance = estimatePerformanceDuringOutages(outageTime, surgery.surgeryTime);
        cout << "Expected performance of " << surgery.teamName << " during power outages: " << performance << "%" << endl;
    }

    double totalOutageTime = calculateTotalOutageTime(outageTime, numberOfTeams);
    cout << "Total amount of time surgery teams will be without power during the next year: " << totalOutageTime << " hours." << endl;

    int totalCustomerHourInterruptions = calculateTotalCustomerHourInterruptions(outageTime, numberOfTeams);
    cout << "Expected total number of customer hour interruptions during the next year: " << totalCustomerHourInterruptions << endl;

    return 0;
}

	

