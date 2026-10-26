#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Patient {
	int id;
	std::string name;
	int priority;
};

struct ComparePriority {
	bool operator()(const Patient &a, const Patient &b) const
	{
		return a.priority < b.priority;
	}
};

class Hospital {
    private:
	std::queue<Patient> patientsQueue;
	std::priority_queue<Patient, std::vector<Patient>, ComparePriority> emergencyQueue;
	int nextPatientId = 1;

    public:
	void registerPatient()
	{
		Patient patient{};

		std::cout << "Enter patient name: ";
		std::cin >> patient.name;
		if (patient.name.empty()) {
			std::cout << "Patient name cannot be empty.\n";
			return;
		}

		patient.id = nextPatientId;

		std::cout << "Enter priority (0-5): ";
		if (!(std::cin >> patient.priority)) patient.priority = 0;

		if (patient.priority < 0 || patient.priority > 5) {
			std::cout << "Enter valid Patient priority 0 to 5.\n";
			return;
		}

		if (patient.priority > 0)
			emergencyQueue.push(patient);
		else
			patientsQueue.push(patient);

		nextPatientId++;

		std::cout << "Patient registered successfully.\n"
			  << "Patient ID: " << patient.id << "\n";
	}

	void serverPatient()
	{
		if (!emergencyQueue.empty()) {
			Patient patient = emergencyQueue.top();
			emergencyQueue.pop();

			std::cout << "Emergency patient consulted.\n"
				  << "ID: " << patient.id << "\n"
				  << "Name: " << patient.name << "\n";
		} else if (!patientsQueue.empty()) {
			Patient patient = patientsQueue.front();
			patientsQueue.pop();

			std::cout << "Regular patient consulted.\n"
				  << "ID: " << patient.id << "\n"
				  << "Name: " << patient.name << "\n";
		} else {
			std::cout << "No patients are waiting.\n";
		}
	}

	void displayPatients() const
	{
		if (patientsQueue.empty() && emergencyQueue.empty()) {
			std::cout << "No patients are waiting.\n";
			return;
		}

		auto emergency = emergencyQueue;

		std::cout << "\n-- Emergency Patients Queue --\n";
		while (!emergency.empty()) {
			const Patient &patient = emergency.top();

			std::cout << "ID: " << patient.id << " | Name: " << patient.name
				  << " | Priority: " << patient.priority << "\n";

			emergency.pop();
		}

		auto waiting = patientsQueue;

		std::cout << "\n-- Patients Queue --\n";
		while (!waiting.empty()) {
			const Patient &patient = waiting.front();

			std::cout << "ID: " << patient.id << " | Name: " << patient.name << "\n";

			waiting.pop();
		}
	}

	void searchPatient() const
	{
		int id;
		std::cout << "Enter patient ID to search: ";
		if (!(std::cin >> id) || id < 0) {
			std::cout << "Invalid patient ID.\n";
			return;
		}

		auto waiting = patientsQueue;
		auto emergency = emergencyQueue;

		while (!waiting.empty()) {
			const Patient &patient = waiting.front();

			if (patient.id == id) {
				std::cout << "Patient found.\n"
					  << "ID: " << patient.id << "\n"
					  << "Name: " << patient.name << "\n"
					  << "Type: Regular\n";
				return;
			}
			waiting.pop();
		}
		while (!emergency.empty()) {
			const Patient &patient = emergency.top();

			if (patient.id == id) {
				std::cout << "Patient found.\n"
					  << "ID: " << patient.id << "\n"
					  << "Name: " << patient.name << "\n"
					  << "Priority: " << patient.priority << "\n"
					  << "Type: Emergency\n";
				return;
			}
			emergency.pop();
		}

		std::cout << "Patient not found in waiting list.\n";
	}
};

void printMenu()
{
	std::cout << "\n-- Hospital Patient Management System -- \n"
		  << "1. Register patient\n"
		  << "2. Consult next patient\n"
		  << "3. Display waiting list\n"
		  << "4. Search patient by ID\n"
		  << "5. Exit\n"
		  << "Choice: ";
}

int main()
{
	Hospital hospital;
	int choice = 0;

	while (choice != 4) {
		printMenu();

		if (!(std::cin >> choice)) {
			std::cout << "Invalid input. Enter a number.\n";
			continue;
		}

		switch (choice) {
			case 1:
				hospital.registerPatient();
				break;
			case 2:
				hospital.serverPatient();
				break;
			case 3:
				hospital.displayPatients();
				break;
			case 4:
				hospital.searchPatient();
				break;
			case 5:
				std::cout << "Exiting hospital system.\n";
				break;

			default:
				std::cout << "Invalid choice. Select 1 to 5.\n";
				break;
		}
	}

	return 0;
}
