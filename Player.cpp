#include <iostream>
#include <fstream>
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

	std::string getPosition() {
		return (*this).position;
	}

private:
	std::string name{};
	std::string position{};
	int player_number{};
	int* die_faces;
};

class Team {
public:
	Player** forwards;
	Player** defenders;
	Player* goalie;

	Team(Player** forwards, Player** defenders, Player* goalie) {
		this->forwards = forwards;
		this->defenders = defenders;
		this->goalie = goalie;
	}

	bool addForward(Player* forward) {
		if ((*forward).getPosition() != "FWD") {
			return false;
		}

		if (num_forwards == 3) {
			return false;
		}

		for (int i = 0; i < num_forwards; i++) {
			if ((*forward).getName() == (*(forwards[i])).getName()) {
				return false;
			}
		}

		forwards[num_forwards] = forward;
		num_forwards++;
	}

	bool removeForward(Player* forward) {
		if ((*forward).getPosition() != "FWD") {
			return false;
		}

		if (num_forwards == 0) {
			return false;
		}

		// Go through forwards, find matching name, put last forward in their position, delete last forward
		for (int i = 0; i < num_forwards; i++) {
			if ((*forward).getName() == (*(forwards[i])).getName()) {
				forwards[i] = forwards[num_forwards - 1];
				forwards[num_forwards - 1] = nullptr;
				break;
			}
		}

		num_forwards--;
	}

private:
	int num_forwards = 0;
	int num_defenders = 0;
	int num_goalies = 0;
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

int game(Team* team1, Team* team2) {
	int field_position = 0;
	int possession = 0;
	int timer = 25 * 60;
	int score_1 = 0, score_2 = 0;

	Player* centre_forward1[] = {(*team1).forwards[1]};
	Player* centre_forward2[] = {(*team2).forwards[1]};
	Player* goalie1[] = {(*team1).goalie};
	Player* goalie2[] = {(*team2).goalie};

	int b;

	while (timer > 0) {
		std::cin >> b;
		std::cout << "The current field position is " << field_position << ". The possession is " << possession << "." << std::endl;
		std::cout << "The score is " << score_1 << " - " << score_2 << std::endl;
		int result;
		if (field_position == 0) {
			result = battle((*team1).forwards,3, (*team2).forwards, 3);		//forwards
		} else if (field_position == 1) {
			result = battle((*team1).forwards, 3, (*team2).defenders, 3);		// your forwards, their defenders
		} else if (field_position == 2) {
			result = battle(centre_forward1, 1, goalie2, 1);		// your center forward, their goalie
		} else if (field_position == -1) {
			result = battle((*team1).defenders, 3, (*team2).forwards, 3);		// your defenders, their forwards
		} else if (field_position == -2) {
			result = battle(goalie1, 1, centre_forward2, 1);		// your goalie, their center forward
		}

		// Determine effect of battle
		if (possession == 1) {
			if (result == 1) {
				field_position++;
			} else if (result == -1) {
				if (field_position == 2) {
					field_position--;
				}
				possession = -1;
			}
		} else if (possession == -1) {
			if (result == 1) {
				if (field_position == -2) {
					field_position++;
				}
				possession = 1;
			} else if (result == -1) {
				field_position--;
			}
		} else {
			// If we win (1), we get possession (1)
			// If we lose (-1), they get possession (-1)
			possession = result;
		}

		// Determine if goal was scored
		if (field_position == 3) {
			score_1++;
			field_position = 0;
			possession = -1;
		} else if (field_position == -3) {
			score_2++;
			field_position = 0;
			possession = 1;
		}

		timer -= (rand() % 30) + 30;
	}

	if (score_1 > score_2) {
		return 1;
	} else if (score_1 < score_2) {
		return -1;
	} else {
		return 0;
	}
}

int main() {
	srand(time(NULL));

	int faces6[6] = {1,2,2,3,3,4};
	Player* david_little = new Player("David Little", "FWD", 6, faces6);
	int faces7[6] = {3,3,4,4,5,6};
	Player* justin_huff = new Player("Justin Huff", "FWD", 7, faces7);
	int faces14[6] = {1,2,2,3,3,4};
	Player* barney_hulligan = new Player("Barney Hulligan", "FWD", 14, faces14);

	// Player* your_players[3]{david_little, justin_huff, barney_hulligan};
	Player* forwards[3] = {david_little, justin_huff, barney_hulligan};
	Team* team1 = new Team(forwards, forwards, david_little);
	Team* team2 = new Team(forwards, forwards, barney_hulligan);

	std::cout << game(team1, team2) << std::endl;

	delete david_little;
	delete justin_huff;
	delete barney_hulligan;
}