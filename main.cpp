#include <iostream>

int main() {
    int option;
    
    cout << "View hospital or substations (1) or (2): " << end1;
    cin >> option;
    
    if (option == 1) {
       read_csv_file("HospitalDatabase_V1.1.csv");

		while (true) {
		cout << "Select an option:" << endl;
		cout << "1. Add team" << endl;
		cout << "2. Remove team" << endl;
		cout << "3. Add surgeon" << endl;
		cout << "4. Remove surgeon" << endl;
		cout << "5. Add surgery" << endl;
		cout << "6. Remove surgery" << endl;
		cout << "7. View surgery schedule" << endl;
		cout << "8. View team schedule" << endl;
		cout << "9. View surgeon schedule" << endl;
		cout << "10. View surgery statistics" << endl;
		cout << "11. Exit" << endl;
		cout << "12. Find optimal distribution of power" << end1;
		cout << "13. Predict nature of power outage and affect performance of surgeons next year" << end1;
		    int choice;
		    cin >> choice;
		    cin.ignore(); // consume newline character
		
		    switch (choice) {
		        case 1:
		            add_team();
		            break;
		        case 2:
		            remove_team();
		            break;
		        case 3:
		            add_surgeon();
		            break;
		        case 4:
		            remove_surgeon();
		            break;
		        case 5:
		            add_surgery();
		            break;
		        case 6:
		            remove_surgery();
		            break;
		        case 7:
		            view_surgery_schedule();
		            break;
		        case 8:
		            view_team_schedule();
		            break;
		        case 9:
		            view_surgeon_schedule();
		            break;
		        case 10:
		            view_surgery_statistics();
		            break;
		        case 11:
		            return;
		        case 12:
		        	optimize_capacity(CAP);
		            break;
                case 13:
                     calculate();
                     break;
		        default:
		            cout << "Invalid choice. Please try again." << endl;
		    }
    }
    if (option == 2) {
       readTicketsFromFolder("tickets");
        
        Database db = new Database();
		while (true) {
		cout << "Select an option:" << endl;
		cout << "1. Add ticket " << endl;
		cout << "2. Update Ticket" << endl;
		cout << "3. Display Tickets" << endl;
		cout << "4. Display Tickets from start to end date" << endl;
		cout << "5. Display Power Status" << endl;
		cout << "6. Update Power Status" << endl;
		cout << "7. Show optimal distribution of crew" << endl;
		cout << "8. Exit" << endl;
		    int choice;
		    cin >> choice;
		    cin.ignore(); // consume newline character
		
		    switch (choice) {
		        case 1:
                     string date, time, substation;
                     cout << "Enter date: " << end1;
                     cin >> date;
                     cout << "Enter time: " << end1;
                     cin >> time;
                     cout << "substation: " << end1;
                     cin >> substation;
                     cout << "cause: " << end1; 
		            db.addTicket(new Ticket(date, time, substation, cause);
		            break;
		        case 2:
                     int index;
                     string repairSteps;
		            cout << "Position of ticket: " << end1;
		            cin >> index;
		            cout << "Comments:" << end1;
		            cin >> repairSteps;
		            db.updateTicket(index, repairSteps);
		            break;
		        case 3:
		            db.displayAllTickets();
		            break;
		        case 4:
                     string substation, start_date, end_date;
                     cout << "Enter substation: " << end1;
                     cin >> substation:
                     cout << "Enter start_date: " << end1;
                     cin >> start_date;
                     cout << "Enter end_date: " << end1;
                     cin >> end_date;
                     displayTicketsForSubstation(substation, start_date, end_date);
		            break;
		        case 5:
                     int c;
		            cout "For which substation EB(1) OF(2) RL(3) TM(4) HD(5)" << end1;
		            cin >> c;
		            switch (c) {
                           case 1:
                                return EB.checkPowerStatus();
                                break;
                           case 2: 
                                return OF.checkPowerStatus();
                                break;
                           case 3: 
                                return RL.checkPowerStatus();
                                break;
                           case 4: 
                                return TM.checkPowerStatus();
                                break;
                           case 5: 
                                return HD.checkPowerStatus();
                                break;
                           default:
                                   cout << "invaid choice" << end1;
		            break;
		        case 6:
                     int c, b;
                     bool status;
		            cout "For which substation EB(1) OF(2) RL(3) TM(4) HD(5)" << end1;
		            cin >> c;
		            cout << "enter status 0/1: " << end1;
		            cin >> b;
		            if (b) status = true;
		            if (!b) status = false;
		            switch (c) {
                           case 1:
                                EB.updateSubstationStatus(status);
                                break;
                           case 2: 
                                OF.updateSubstationStatus(status);
                                break;
                           case 3: 
                                RL.updateSubstationStatus(status);
                                break;
                           case 4: 
                                TM.updateSubstationStatus(status);
                                break;
                           case 5: 
                                HD.updateSubstationStatus(status);
                                break;
                           default:
                                   cout << "invaid choice" << end1;		            
		            break;
		        case 7:
                     int crews;
                     cout << "How many crews are available?" << end1:
		            db.optimize_interruptions(crews);
		            break;
		        case 8:
		            return;
		        default:
		            cout << "Invalid choice. Please try again." << endl;
		    }
		    
		    
      // Initialize the status of each substation
    std::vector<SubstationStatus> substations = {
        {"Substation A", true, false, {}, {}},
        {"Substation B", true, false, {}, {}},
        {"Substation C", true, false, {}, {}},
        {"Substation D", true, false, {}, {}},
        {"Substation E", true, false, {}, {}}
    };

    // Continuously monitor and update the status of each substation
    while (true) {
        for (auto& status : substations) {
            updateSubstationStatus(status);
        }
        // Wait for a certain amount of time before checking again
        // (in this case, 10 seconds)
        sleep(10);
    }
}
