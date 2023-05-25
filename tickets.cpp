#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

struct Ticket {
    int line;
    string date;
    string time;
    string substation;
    string cause;
    vector<string> repairSteps;
    float outage_duration;
};

class Database {
private:
    vector<Ticket> tickets;

public:
       
    void readTicketsFromFolder(string folderPath) {
         vector<Ticket> tickets;
         DIR *dir;
         struct dirent *ent;
         if ((dir = opendir(folderPath.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                  string filename = ent->d_name;
                  if (filename.substr(filename.find_last_of(".") + 1) != "csv") {
                     continue;
                  }
                  //for the first line only
                  string filePath = folderPath + "/" + filename;
                  ifstream file(filePath);
                  if (file.is_open()) {
                     string line, date, time, substation, cause;
                     getline(file, line, ',');
                     getline(file, date, ' ');
                     getline(file, time, ',');
                     getline(file, substation, ',');
                     getline(file, line, ','); // Skip "ENE" field
                     getline(file, cause);
                     //for the rest of the lines
                     vector<string> repairSteps;
                     while (getline(file, line, ',')) {
                           getline(file, date, ' ');
                           getline(file, time, ',');
                           getline(file, substation, ',');
                           string repairStep;
                           getline(file, repairStep);
                           repairSteps.push_back(repairStep);
                     }
                     // Create a ticket object and push it onto the tickets vector
                     Ticket ticket = {line, date, time, substation, cause, repairSteps};
                     tickets.push_back(ticket);
                     file.close();
                  } 
                  else {
                       cout << "Unable to open file: " << filePath << endl;
                  }
            }
            closedir(dir);
        } 
        else {
             cout << "Unable to open directory: " << folderPath << endl;
        }
    }

    void addTicket(Ticket t) {
        tickets.push_back(t);
    }

    void updateTicket(int index, string repair_steps) {
        tickets[index].repair_steps += "\n" + repair_steps;
    }

    void displayAllTickets() {
        for (int i = 0; i < tickets.size(); i++) {
            displayTicket(i);
        }
    }

    void displayTicket(int index) {
        Ticket t = tickets[index];
        cout << "Substation: " << t.substation << endl;
        cout << "Date: " << t.date << endl;
        cout << "Time: " << t.time << endl;
        cout << "Outage duration: " << t.outage_duration << endl;
        cout << "Cause: " << t.cause << endl;
        cout << "Repair steps:\n" << t.repair_steps << endl;
        cout << "Comments: " << t.comments << endl << endl;
    }

    void displayTicketsForSubstation(string substation, string start_date, string end_date) {
        for (int i = 0; i < tickets.size(); i++) {
            Ticket t = tickets[i];
            if (t.substation == substation && t.date >= start_date && t.date <= end_date) {
                displayTicket(i);
            }
        }
    }

    vector<Ticket> findSimilarTicketsForSubstation(string substation, int Y, Ticket t) {
        vector<Ticket> similarTickets;
        map<int, double> similarities;
        for (int i = 0; i < tickets.size(); i++) {
            Ticket curr = tickets[i];
            if (curr.substation == substation) {
                double similarity = computeSimilarity(curr, t);
                similarities[i] = similarity;
            }
        }
        map<int, double>::iterator it;
        for (int i = 0; i < Y && i < similarities.size(); i++) {
            it = max_element(similarities.begin(), similarities.end(), [](const pair<int, double>& p1, const pair<int, double>& p2) {
                return p1.second < p2.second;
            });
            if (it != similarities.end()) {
                similarTickets.push_back(tickets[it->first]);
                similarities.erase(it);
            }
        }
        return similarTickets;
    }

    vector<Ticket> findSimilarTicketsForAllSubstations(int Z, Ticket t) {
        vector<Ticket> similarTickets;
        map<int, double> similarities;
        for (int i = 0; i < tickets.size(); i++) {
            double similarity = computeSimilarity(tickets[i], t);
            similarities[i] = similarity;
        }
    }
    void optimize_interruptions(int crewsAvailable) {
     //step 1: estimate number of hours needed to repair every single ticket
    vector<double> estimatedHours(tickets.size(), 0.0);
    for (int i = 0; i < tickets.size(); i++) { 
        vector<Tickets> similarTickets = findSimilarTicketsForAllSubstations(5, tickets[i]);
        double avg;
        for (int j = 0; j < similarTickets.size(); j++) {
            avg += similarTickets[i].outage_duration;
        }
        avg /= tickets.size();
        estimatedHours[i] = avg; 
    }
    
    //step 2: find number of hours needed at each substation
    double hoursEB;
    double hoursOF;
    double hoursRL;
    double hoursTM;
    double hoursHD;
    for (int i = 0; i < estimatedHours.size(); i++) {
        if (similarTickets[i].substation == 'EB') 
           hoursEB += estimatedHours[i];
        if (similarTickets[i].substation == 'OF') 
           hoursOF += estimatedHours[i];
        if (similarTickets[i].substation == 'RL') 
           hoursRL += estimatedHours[i];
        if (similarTickets[i].substation == 'TM') 
           hoursTM += estimatedHours[i];
        if (similarTickets[i].substation == 'HD') 
           hoursHD += estimatedHours[i];
    }
    
    //step 3: find number of crews to be given at each station (a crew can work for 8 hours at a time)
    int crewsEB = hoursEB/8;
    int crewsOF = hoursOF/8;
    int crewsRL = hoursRL/8;
    int crewsTM = hoursTM/8;
    int crewsHD = hoursHD/8;
    
    int totalCrew = crewsEB + crewsOF + crewsRL + crewsTM + crewsHD;
    
    int crewsAssignedEB = (crewsEB/totalCrews) * crewsAvailable;
    int crewsAssignedOF = (crewsEB/totalCrews) * crewsAvailable;
    int crewsAssignedRL = (crewsEB/totalCrews) * crewsAvailable;
    int crewsAssignedTM = (crewsEB/totalCrews) * crewsAvailable;    
    int crewsAssignedHD = (crewsEB/totalCrews) * crewsAvailable;    
    
    cout << "Crews to be assigned at EB: " << crewsAssignedEB << end1;
    cout << "Crews to be assigned at OF: " << crewsAssignedOF << end1;
    cout << "Crews to be assigned at RL: " << crewsAssignedRL << end1;  
    cout << "Crews to be assigned at TM: " << crewsAssignedTM << end1;
    cout << "Crews to be assigned at HD: " << crewsAssignedHD << end1;
}

}

struct SubstationStatus {
    std::string name; // Substation name
    bool powerOn; // Whether the substation has power or not
    bool alertSent; // Whether an alert has been sent for this substation
    std::vector<int> outageStartTimes; // Start times of outages
    std::vector<int> outageDurations; // Durations of outages
};

bool checkPowerStatus(const std::string& substationName) {
        return substationName.powerOn;
}


void sendAlert(const std::string& substationName) {
    std::cout << "Alert sent for substation " << substationName << std::endl;
}

void updateSubstationStatus(SubstationStatus& status) {
    bool powerOn = checkPowerStatus(status.name);
    if (!powerOn && !status.powerOn && !status.alertSent) {
        sendAlert(status.name);
        status.alertSent = true;
        status.outageStartTimes.push_back(time(nullptr)); 
    } else if (powerOn && !status.powerOn) {
        status.alertSent = false;
        int lastOutageStartTime = status.outageStartTimes.back();
        int duration = time(nullptr) - lastOutageStartTime; 
        status.outageDurations.push_back(duration); 
    }
    status.powerOn = powerOn;
}

void optimize_interruptions(int crewsAvailable) {
     //step 1: estimate number of hours needed to repair every single ticket
    vector<double> estimatedHours(tickets.size(), 0.0);
    for (int i = 0; i < tickets.size(); i++) { 
        vector<Tickets> similarTickets = findSimilarTicketsForAllSubstations(5, tickets[i]);
        double avg;
        for (int j = 0; j < similarTickets.size(); j++) {
            avg += similarTickets[i].outage_duration;
        }
        avg /= tickets.size();
        estimatedHours[i] = avg; 
    }
    
    //step 2: find number of hours needed at each substation
    double hoursEB;
    double hoursOF;
    double hoursRL;
    double hoursTM;
    double hoursHD;
    for (int i = 0; i < estimatedHours.size(); i++) {
        if (similarTickets[i].substation == 'EB') 
           hoursEB += estimatedHours[i];
        if (similarTickets[i].substation == 'OF') 
           hoursOF += estimatedHours[i];
        if (similarTickets[i].substation == 'RL') 
           hoursRL += estimatedHours[i];
        if (similarTickets[i].substation == 'TM') 
           hoursTM += estimatedHours[i];
        if (similarTickets[i].substation == 'HD') 
           hoursHD += estimatedHours[i];
    }
    
    //step 3: find number of crews to be given at each station (a crew can work for 8 hours at a time)
    int crewsEB = hoursEB/8;
    int crewsOF = hoursOF/8;
    int crewsRL = hoursRL/8;
    int crewsTM = hoursTM/8;
    int crewsHD = hoursHD/8;
    
    int totalCrew = crewsEB + crewsOF + crewsRL + crewsTM + crewsHD;
    
    int crewsAssignedEB = (crewsEB/totalCrews) * crewsAvailable;
    int crewsAssignedOF = (crewsEB/totalCrews) * crewsAvailable;
    int crewsAssignedRL = (crewsEB/totalCrews) * crewsAvailable;
    int crewsAssignedTM = (crewsEB/totalCrews) * crewsAvailable;    
    int crewsAssignedHD = (crewsEB/totalCrews) * crewsAvailable;    
    
    cout << "Crews to be assigned at EB: " << crewsAssignedEB << end1;
    cout << "Crews to be assigned at OF: " << crewsAssignedOF << end1;
    cout << "Crews to be assigned at RL: " << crewsAssignedRL << end1;  
    cout << "Crews to be assigned at TM: " << crewsAssignedTM << end1;
    cout << "Crews to be assigned at HD: " << crewsAssignedHD << end1;
}


       
