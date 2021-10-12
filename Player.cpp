#include <iostream>
#include <time.h>

class Player {
public:
	Player(std::string name, std::string position, int player_number, int* die_faces) {
		(*this).name = name;
		this->position = position;
		this->player_number = player_number;
		this->die_faces = die_faces;
	}

	std::string getName() {
		return (*this).name;
	}

	int roll() {
		return this->die_faces[(int)(rand() % 6)];
	}

	void setName(std::string newName) {
		(*this).name = newName;
	}

private:
	std::string name{};
	std::string position{};
	int player_number{};
	int* die_faces;
};

int battle(Player** your_players, int your_num, Player** opp_players, int opp_num) {
	if (your_num != opp_num) {
		std::cout << "Your num of players (" << your_num << ") did not match opponent's num (" << opp_num << ")" << std::endl;
		exit(-1);
	}
	int your_sum = 0;
	for (int i = 0; i < your_num; i++) {
		your_sum += (*your_players[i]).roll();
	}

	int opp_sum = 0;
	for (int i = 0; i < opp_num; i++) {
		opp_sum += (*opp_players[i]).roll();
	}

	std::cout << your_sum << " " << opp_sum << std::endl;

	if (your_sum > opp_sum) {
		return 1;
	} else if (your_sum < opp_sum) {
		return -1;
	} else {
		return 0;
	}
}

int main() {
	srand(time(NULL));

	int faces[6] = {1,2,2,3,3,4};
	Player* david_little = new Player("David Little", "FWD", 6, faces);
	Player* david_little2 = new Player("David Little", "FWD", 6, faces);
	Player* david_little3 = new Player("David Little", "FWD", 6, faces);

	Player* your_players[3]{david_little, david_little2, david_little3};

	std::cout << battle(your_players, 3, your_players, 2) << std::endl;

	delete david_little;
	delete david_little2;
	delete david_little3;
}