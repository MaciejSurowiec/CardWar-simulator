#include <iostream>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define CARDS 52
#define COLORS 4
#define RANKS 13
#define BORDER 500
#define RANGA 0.1

using namespace std;

//Obiekt pojedynczej karty
typedef struct Card {
protected:

	int8_t color;
	int8_t value;

public:
	// Pik->1 Trefl->2 Karo->3 Kier->4
	int8_t GetColor() { return color; }
	//  1->2 ... 10->9 J->10 Q->11 K->12 As->13
	// pierwsze 7 najstarszych kart ma range cala talia ma 280 punktow
	//ranga karty A->13 K->12 Q->11 .... 8->7

	Card() { ; }//kostruktor pustej karty
	Card(int8_t col, int8_t val)//konstruktor pelnej karty
	{
		color = col;
		value = val;
	}
	int8_t GetValue() { return value; }
	void SetValue(int8_t Value) { value = Value; }
	void SetColor(int8_t Color) { color = Color; }
	void Print()
	{
		if (value < 10 && value > 0)
		{
			cout << (int)value + 1;
		}
		else
		{
			switch (value)
			{
			case 10:
				cout << 'J';
				break;
			case 11:
				cout << 'Q';
				break;
			case 12:
				cout << 'K';
				break;
			case 13:
				cout << 'A';
				break;
			default:
				cout << "0";
				break;
			}
		}
		cout << ' ';

		switch (color)
		{
		case 1:
			cout << "Pik";
			break;
		case 2:
			cout << "Trefl";
			break;
		case 3:
			cout << "Karo";
			break;
		case 4:
			cout << "Kier";
			break;

		default:
			cout << "0";
			break;
		}
		//cout << endl;
	}

}Card;

//Obiekt talii
struct Deck {
protected:
	Card* cards;
public:
	Deck()
	{
		cards = new Card[CARDS];
		for (int8_t i = 0; i < COLORS; i++)
		{
			for (int8_t j = 0; j < RANKS; j++)
			{
				cards[(i * RANKS) + j].SetColor(i + 1);
				cards[(i * RANKS) + j].SetValue(j + 1);
			}
		}
	}

	// ile kart ma byc w tali liczone od asa w dol
	Deck(int8_t n)
	{
		cards = new Card[CARDS];
		for (int8_t i = 0; i < COLORS; i++)	
		{
			for (int8_t j = 0; j < RANKS; j++)
			{
				cards[(i * RANKS) + j].SetColor((RANKS - n) <= j ? (i + 1) : 0);
				cards[(i * RANKS) + j].SetValue((RANKS - n) <= j ? (j + 1) : 0);
			}
		}
	}
	//pobieranie karty
	Card * GetCard(int8_t n) { return cards + n; }

	//tasowanie kart
	void Shuffle()
	{
		// korzystam z g³ównej tali i wklejam jej elementy do nowej tablicy na koniec podmieniam wskazniki
		Deck* temp;
		temp = new Deck(0);
		time_t clock;
		srand(time(&clock));
		int8_t i = 0;

		while (i < CARDS)
		{
			Card t = cards[i];// aktualna karta dla której szukamy wolnego miejsca w nowej tablicy
			int8_t r = rand() % (CARDS - i);

			int8_t g = 0;
			while (g < CARDS)
			{
				if (temp->cards[g].GetValue() == 0)
				{
					if (r == 0)
					{
						temp->cards[g].SetValue(t.GetValue());
						temp->cards[g].SetColor(t.GetColor());
						break;
					}
					else r--;
				}

				g++;
			}

			i++;
		}

		Card* dest = cards;
		cards = temp->cards;
		free(dest);
	}
	void PrintDeck()
	{
		for (int8_t i = 0; i < CARDS; i++)
		{
			cards[i].Print();
			cout << endl;
		}
	}

	~Deck() {free(cards);}
};


typedef struct ListElement {
	Card* card;
	ListElement* Next;

	ListElement(Card* cardwsk)
	{
		Next = NULL;
		card = cardwsk;
	}

}ListElement;


typedef struct Hand{
protected:
	ListElement* first;
	ListElement* last;
	//ilosc elementow czyli indeks +1
	int8_t size;
public:
	Hand() { size = 0; first = NULL; last = NULL; }
	int8_t GetSize() { return size; }
	Card* GetCard() { return first->card; }//pobieranie karty
	Card* GetSecond() { return first->Next->card; }//pobieranie karty 2 w kolejnosci

	//dodawanie karty na tyl kolejki
	void Push(Card* card)
	{
		ListElement* list;
		list = new ListElement(card);

		if (size == 0)
		{
			first = list;
			last = list;
		}
		else
		{
			last->Next = list;
			last = list;
		}
		
		size++;
	}
	//dodawanie karty na przod kolejki
	void Front(Card* card)
	{
		ListElement* list;
		list = new ListElement(card);

		if (size == 0)
		{
			first = list;
			last = list;
		}
		else
		{
			list->Next = first;
			first = list;
		}

		size++;
	}

	//usuwanie pierwszego elementu kolejki
	void Pop()
	{
		ListElement* temp = first;
		first = first->Next;
		free(temp);
		size--;
	}
	//wyswietlanie calej reki
	void Print()
	{
		ListElement* temp = first;
		for (int i = 0; i < size; i++)
		{
			cout << i << ' ';
			temp->card->Print();

			temp = temp->Next;
			cout << endl;
		}
	}

	~Hand()
	{
		while (size)
		{
			Pop();
		}
	}

}Hand;


//Obiekt Gracza
struct Player {
private:

	Hand hand;
	bool help;
	int rank;
	int8_t as;
	int8_t mode;
	//0-pokojowy 1-agresywny 2-losowy

	Card* Ret(Card* a, Card* b)
	{
		hand.Front(b);
		return a;
	}

public:

	Player(int h)
	{
		help = true;
		rank = 0;
		as = 0;
		mode = h;
	}

	bool GetHelp() { return help; }
	Hand* GetHand() { return &hand; }
	void SetHelp(bool h) { help = h; }
	int GetRank() { return rank; }
	int8_t GetAs() { return as; }
	void AddToRank(float t) { rank += t; }
	void AddtoAs() { as++; }//dodawanie asa
	void Getmode() { cout << (int)mode; }
	void SetMode(int numer) { mode = numer; }

	Card* Choose(Card* card, bool pl)//karta zagrana przez przeciwnika
	{
		Card* a = hand.GetCard();
		hand.Pop();
		if (pl)
		{
			if (hand.GetSize() >= 2)//sprawdza czy gracz ma wgl w czym wybierac
			{
				if (mode != 0)
				{
					Card* b = hand.GetCard();
					hand.Pop();

					if (b->GetValue() > a->GetValue())// sprawia ze a jest zawsze silniejsza karta
					{
						Card* temp = b;
						b = a;
						a = temp;
					}

					if (mode == 1)//agresywny
					{
						if (card->GetValue() == a->GetValue()) Ret(a, b);
						else
						{
							if (card->GetValue() == b->GetValue()) Ret(b, a);//jezeli a nie tworzy wojny a b tworzy wojne lub wygrywa bitwe to zostanie zwrocone
							else
							{
								if (card->GetValue() < a->GetValue())
								{
									if (card->GetValue() < b->GetValue()) Ret(b, a);
									else Ret(a, b);
								}
								else Ret(b, a);
							}
						}
					}
					else//pokojowy
					{
						if (card->GetValue() == a->GetValue()) Ret(b, a); //unkanie wojny 

						if (card->GetValue() == b->GetValue())Ret(a, b); //unkanie wojny 
						//wystawia b bo jezeli one przebije przeciwnika to a tez go przebija
						Ret(b, a);
					}
				}
				else//losowy
				{
					if (rand() % 2) return a;
					else
					{
						Card* b = hand.GetCard();
						hand.Pop();
						Ret(b, a);
					}
				}
			}
			else return a;//zwracanie 1 karty jezeli jest to ostatania karta
		}
		else// interfejs graficzny 
		{
			Card* b = hand.GetCard();
			hand.Pop();
			cout << "karta przeciwnika: ";
			card->Print();
			cout <<endl<< "Wybierz karte:" << endl;
			cout << "0- ";
			a->Print();
			cout << endl << "1- ";
			b->Print();
			cout << endl;
			char g;
			cin >> g;
			if (g < '2' || g >= '0')
			{
				if (g-'0')
				{
					hand.Front(a);
					b->Print();
					cout << endl;
					return b;
				}
				else
				{
					hand.Front(b);
					cout << endl;
					return a;
				}
			}
		}
	}
};


struct Menu {
protected:
	int quantity;//ilosc symulacji do wykonania
	bool gamevariant;//tryb gry (A lub B)
	bool quit;//czy zakonczyc porogram
	bool dumb;//wojna zwykla 1 madra 0
	bool eksp;//0-gra z ijnterfejsem tekstowym 1- wersja eksperymentalna
	int8_t number;//ilosc kart w tali
	int8_t playero;// strategia poszczegolnych graczy 0-losowy 1-wsciekly 2-pokojowy
	int8_t playert;

public:

	Menu()
	{
		quit = true;
		gamevariant = true;

		dumb = true;
		number = 0;
		quantity = 2;
		
	}

	bool GetVariant() { return gamevariant; }
	bool GetQuit() { return quit; }
	short GetQuantity() { return quantity; }
	bool GetMind() { return dumb; }
	int8_t GetNumber() { return number; }
	bool GetGame() { return eksp; }

	//wszystkie funkcje mowia co robia w 1 coucie
	void SetVariant()
	{
		cout << "Podaj A lub B aby wybrac tryb gry:";
		char c;
		cin >> c;
		if (c == 'A' || c == 'a') gamevariant = true;
		else if (c == 'B' || c == 'b') gamevariant = false;
		else SetVariant();
	}

	void MindSet()
	{
		cout << "czy wojna ma byc madra czy nie (0-madra 1-nie) lub nacisnij Q aby wyjsc:";
		char c;
		cin >> c;
		if (c == 'Q' || c == 'q') quit = false;
		else if (c > '2' || c < '0') MindSet();
		dumb = (c-'0');
	}

	void GameSet()
	{
		cout << "1 - wersja eksperymentalna do badan | 0 - werjsa do gry z komputerem | Q -  wyjscie z programu:";
		char c;
		cin >> c;
		if (c == 'Q' || c == 'q') quit = false;
		else if (c > '2' || c < '0') GameSet();
		eksp = (c - '0');

	}

	void SetQuantity()
	{
		cout << "podaj ilosc symulacji do wykonania:";
		cin >> quantity;
	}

	void SetNumber()
	{
		int t;
		cout << "podaj rozmiar tali(13- wszystkie karty,12- pierwsze 12 kart zaczynajac od asa.....):";
		cin >> t;
		if (t > 13 || t < 0)SetNumber();
		else number = t;
	}

	int8_t PlayerMode(int8_t n)
	{
		int numer;
		cout << "wybierz strategie " << (int)n << " gracza (0-losowy 1- agresywna 2-pokojowy):";
		cin >> numer;
		if (numer > 3 && numer< 0)PlayerMode(n);
		else return numer;
	}
};


struct Game {
protected:
	Deck* deck;
	Player* one;
	Player* two;
	bool gamevariant;//true-A false -B
	bool status;//czy gra nadal trwa
	int8_t cards;//ile kart w tali 
	int turns;//licznik tur
	bool infinite;//czy gra nie ma konca na danym ulozeniu 
	bool dumb;// 1-zwykla wojna 0-madra
	bool which_player;//ktory gracz ma ruch w madrej wojnie
	int loser_rank;
	int winner_rank;
	bool first;//czy wygral 1 gracz
	bool comp;// 0- gra z interfejsem graficzny 1-eksperymenty
	bool see;

	struct WAR {//struktura do zapisywania stosu kart w bitwie i wojnie
		Hand battlefield;
		bool winner;
		// zapis do kolejki
		//one,two,one,two.....
		//tru->one fals->two

		WAR(Hand temp_hand, bool temp_winner)
		{
			battlefield = temp_hand;
			winner = temp_winner;
		}

		WAR() { ; }

		void Shuffle()// tasowanie zwracanych kart do madrej wojny
		{
			Hand h;
			while (battlefield.GetSize())
			{		
				if (rand() % 2) h.Push(battlefield.GetCard());
				else h.Front(battlefield.GetCard());

				battlefield.Pop();
			}

			while (h.GetSize())
			{
				battlefield.Push(h.GetCard());
				h.Pop();
			}
		}
	};

	//funkcja uzywana tylko w tym obiekcie to moze byc schowana
	void Add(Player* pl, int8_t g)
	{
		if (deck->GetCard(g)->GetValue() > cards * RANGA)
		{
			pl->AddToRank(deck->GetCard(g)->GetValue()* deck->GetCard(g)->GetValue());
			if (deck->GetCard(g)->GetValue() == RANKS)
			{
				pl->AddtoAs();
				pl->AddToRank(RANKS*RANKS * pl->GetAs());
			}
			
			if (pl->GetHand()->GetSize() > 1)
			{
				if (pl->GetHand()->GetSecond()->GetValue() > cards * RANGA)
				{
					pl->AddToRank(pl->GetHand()->GetSecond()->GetValue() * pl->GetHand()->GetSecond()->GetValue()*RANGA);
				}
			}
		}

		pl->GetHand()->Push(deck->GetCard(g));
	}

	//funkcja oddajaca karty kiedy jakis gracz wygra i sprawdzajaca czy jakis z graczy nie przegral
	void Winner(WAR* war)
	{
		Hand h;
		int8_t i = war->battlefield.GetSize() / 2;
		if (!dumb) war->Shuffle();//tasowanie kart przed oddaniem w madrej wojnie

		if (see)
		{
			if (war->battlefield.GetSize() > 2)cout << "Wojne";
			else cout << "Bitwe";

			cout << " wygrywa gracz ";

			if (war->winner)cout << "1";
			else cout << "2";

			cout << endl << endl;
		}

		while (i)
		{
			if (war->winner)
			{
				one->GetHand()->Push(war->battlefield.GetCard());
				war->battlefield.Pop();
				h.Push(war->battlefield.GetCard());
			}
			else
			{
				h.Push(war->battlefield.GetCard());
				war->battlefield.Pop();
				two->GetHand()->Push(war->battlefield.GetCard());
			}

			war->battlefield.Pop();
			i--;
		}

		

		while (h.GetSize())
		{
			if (war->winner)
			{
				one->GetHand()->Push(h.GetCard());
			}
			else
			{
				two->GetHand()->Push(h.GetCard());
			}

			h.Pop();
		}

		//dawanie od nowa szansy w wariancie B
		if (!one->GetHelp()) { one->SetHelp(true); }

		if (!two->GetHelp()) { two->SetHelp(true); }

		free(war);//usuwanie obiektu zapisujacego karty na stosie
		EndGame();//kiedy po bitwie lub wojnie gracz nie ma kart
	}

	bool DCheck(Player*o,Player *t)
	{
		if (o->GetHand()->GetSize() == 0)
		{
			if (t->GetHand()->GetSize() > 3 && o->GetHelp())
			{
				for (int i = 0; i < 2; i++)
				{
					o->GetHand()->Push(t->GetHand()->GetCard());
					t->GetHand()->Pop();
				}
				o->SetHelp(false);
			}
			else return true;
		}
		else
		{
			if (t->GetHand()->GetSize() < 3) return true;
		}
	
		return false;
	}

	//sprawdzanie czy ktos nie przegrywa podczas wojny i czy mozna pomoc w wariancie B
	void SurvivalTest()
	{
		if (gamevariant)// wariant A
		{
			if (one->GetHand()->GetSize() < 2) SecondWin();

			if (two->GetHand()->GetSize() < 2) FirstWin();
		}
		else //wariant B
		{
			if (one->GetHand()->GetSize() < 2 && two->GetHand()->GetSize() < 2)
			{
				if (one->GetHand()->GetSize() < two->GetHand()->GetSize())SecondWin();
				else FirstWin();
			}
			else
			{
				if (one->GetHand()->GetSize() < 2)
				{
					//sprawdzanie czy 2 gracz ma wystarczajaco kart zeby oddac i czy gracz 1 nie otrzymal juz przypadkiem pomocy
					if (DCheck(one, two))SecondWin();
				}

				if (two->GetHand()->GetSize() < 2)
				{
					if (DCheck(two, one))FirstWin();
				}
			}
		}
	}

	void FirstWin()
	{
		if (!comp || see)cout << "Gracz pierwszy wygral" << endl;

		status = false;
		winner_rank = one->GetRank();
		loser_rank = two->GetRank();
		first = true;
	}

	void SecondWin()
	{
		if (!comp || see) cout << "Gracz drugi wygral" << endl;

		status = false;
		winner_rank = two->GetRank();
		loser_rank = one->GetRank();
	}

	//sprawdzanie ile kart (jezeli trzeba) trzeba przekazac graczowi potrzebujacemu pomocy
	void Check()
	{
		if (one->GetHand()->GetSize() == 0)
		{
			if (one->GetHelp())
			{
				one->GetHand()->Push(two->GetHand()->GetCard());
				two->GetHand()->Pop();
				one->SetHelp(false);
			}
			else SecondWin();
		}

		if (two->GetHand()->GetSize() == 0)
		{
			if (two->GetHelp())
			{
				two->GetHand()->Push(one->GetHand()->GetCard());
				one->GetHand()->Pop();
				two->SetHelp(false);
			}
			else FirstWin();
		}
	}

	//funkcja sprawdzajaca czy gra nie bedzie  trwala w nieskonczonosc
	void InfinityWar()
	{
		if (turns > cards * COLORS * BORDER)
		{
			cout << "war";
			status = false;
			infinite = true;
		}
	}

	//sprawdzanie czy jakis gracz nie przegral
	void EndGame()
	{
		if (one->GetHand()->GetSize() == 0) { SecondWin(); }

		if (two->GetHand()->GetSize() == 0) { FirstWin(); }
	}
	
public:

	//konstruktor
	Game(Deck* d, Player* o, Player* t,Menu *mem)
	{
		deck = d;
		one = o;
		two = t;
		gamevariant = mem->GetVariant();
		status = true;
		turns = 0;
		cards = mem->GetNumber();
		infinite = false;
		winner_rank = 0;
		loser_rank = 0;
		dumb = mem->GetMind();
		which_player = true;
		first=false;
		comp = mem->GetGame();
		see = mem->GetQuantity() == 1 ? 1 : 0;
	}

	Game(Deck* d, Player* o, Player* t, Menu *mem, int8_t car)
	{
		deck = d;
		one = o;
		two = t;
		gamevariant = mem->GetVariant();
		status = true;
		turns = 0;
		cards = car;
		infinite = false;
		winner_rank = 0;
		loser_rank = 0;
		dumb = false;
		which_player = true;
		first = false;
		comp = mem->GetGame();
		see = false;
	}

	bool GetStatus() { return status; }
	bool GetInfinity() { return infinite; }
	int Turns() { return turns; }
	int GetWinner() { return winner_rank; }
	int GetLoser() { return loser_rank; }
	
	

	//rozdawanie kart graczom
	void Draw()
	{
		int j = 0;
		for (int i = 0; i < cards*COLORS; i++)
		{
			while (deck->GetCard(i + j)->GetValue() == 0)j++;

			if (i % 2 == 0)
			{
				Add(one,i+j);
			}
			else
			{
				Add(two, i + j);
			}
		}
	}

	// na poczatku results jest puste, wiec w tej funkcji odpalana jest ta sama ale z parametrem 
	void Battle()
	{
		WAR *results;
		results = new WAR;
		Battle(results);
	}

	//funkcja odpowiedzialna za bitwy i wojny 
	void Battle(WAR * results)
	{
		Card* a, *b;
		if (dumb)
		{
			a = one->GetHand()->GetCard();
			b = two->GetHand()->GetCard();
			one->GetHand()->Pop();
			two->GetHand()->Pop();
			if (see)
			{
				cout << "Karta 1 Gracza: ";
				a->Print();
				cout<<"   Karta 2 Gracza: ";
				b->Print();
				cout << endl<<endl;
			}
			
		}
		else
		{
			if (which_player)
			{	
				a = one->GetHand()->GetCard();
				one->GetHand()->Pop();
				if (two->GetHand()->GetSize() > 1) b = two->Choose(a,true);//tylko 1 grzcz to czlowiek
				else
				{
					b = two->GetHand()->GetCard();
					two->GetHand()->Pop();
				}
				which_player = false;

				if (!comp)
				{
					cout <<endl<< "Twoja karta: ";
					a->Print();
					cout << "    Karta przeciwnika: ";
					b->Print();
					cout << endl<<endl;
				}
			}
			else
			{
				b = two->GetHand()->GetCard();
				two->GetHand()->Pop();
				if(one->GetHand()->GetSize()>1) a = one->Choose(b,comp);
				else
				{
					a = one->GetHand()->GetCard();
					one->GetHand()->Pop();
				}
				which_player = true;
			}
		}
		
		results->battlefield.Push(a);
		results->battlefield.Push(b);

			turns++;
			if (a->GetValue() != b->GetValue())
			{
				if (a->GetValue() > b->GetValue()) results->winner = true;
				else results->winner = false;

				Winner(results);
			}
			else
			{
				// sprawdz czy ma 2 karty niezaleznie od wariantu( w wariancie B sprawdz jeszcze czy przeciwnik moze oddac karty)
				//tylko jezeli ma 0 kart od razu dac mu 2 karty
				//jezeli ma 1 to po oddaniu jej dac mu karte
				SurvivalTest();
				if (status)
				{
					Card* c = one->GetHand()->GetCard();
					Card* d = two->GetHand()->GetCard();
					results->battlefield.Push(c);
					results->battlefield.Push(d);
					one->GetHand()->Pop();
					two->GetHand()->Pop();
					if(!comp)cout<< "Twoja karta: ?    Karta przeciwnika: ?"<<endl;

					if (see)
					{
						cout << "Zakryta karta gracza 1: ";
						c->Print();
						cout << "   Zakryta karta gracza 2: ";
						d->Print();
						cout << endl<<endl;
					}

					if(!gamevariant) Check();//sprawdza czy nie brakuje jakiemus graczowi 1 karty
					Battle(results);
					turns++;
				}
			}
		
		InfinityWar();
	}

	bool Win() { return first; }
	
};


int main()
{
	FILE* fp = fopen("wynik.txt", "a");

	Menu menu;

	int i = RANKS;

		while (true)
		{
			menu.GameSet();
			if (!menu.GetQuit())
			{
				fclose(fp);
				exit(0);
			}

			if (menu.GetGame())
			{
				menu.MindSet();
				
				if (!menu.GetQuit())
				{
					fclose(fp);
					exit(0);
				}

				if (menu.GetMind())menu.SetVariant();

				menu.SetNumber();
				menu.SetQuantity();

				int8_t o = 0, t = 0;

				if (!menu.GetMind())
				{
					o = menu.PlayerMode(1);
					t = menu.PlayerMode(2);
				}
				int procenty = 0;
				long long turns = 0;
				Deck deck(menu.GetNumber());

				for (int j = 0; j < menu.GetQuantity(); j++)
				{
					
					Player one(o);
					Player two(t);
					deck.Shuffle();

					int  winner_rank = 0, loser_rank = 0;

					Game game(&deck, &one, &two,&menu);
					game.Draw();

					if (menu.GetQuantity() == 1)
					{
						one.GetHand()->Print();
						cout << endl;
						two.GetHand()->Print();
						cout << endl;
					}

					while (game.GetStatus())
						game.Battle();

					if (game.GetInfinity())j--;
					else
					{
						turns += game.Turns();
						winner_rank = game.GetWinner();
						loser_rank = game.GetLoser();

						if (!menu.GetMind()) { if (game.Win())procenty++; }
						else if (winner_rank > loser_rank)procenty++;
						
					}
				}

				fprintf(fp, "%d ,%d , %d \n", (int)menu.GetNumber(), (int)procenty*100/menu.GetQuantity(), (int)turns/menu.GetQuantity());
			}
			else
			{
				int8_t t = menu.PlayerMode(2);
				Player two(t);
				Player one(0);//Gracz
				Deck deck(RANKS);
				deck.Shuffle();
				
				Game game(&deck, &one, &two, &menu, RANKS);
				game.Draw();

				

				//dodac do playera konstruktor gracza czlowieka
				// wrzucic do playera zapytania dla gracza jaka karte rzucic
				while (game.GetStatus())
				{
					cout <<"Ilosc kart w rece:"<<(int)one.GetHand()->GetSize()<<endl;
					game.Battle();
				}
				cout <<endl<< game.Turns();
				cout << endl;
			}
		}

		fclose(fp);

	return 0;
}
