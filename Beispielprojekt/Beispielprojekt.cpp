#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <dos.h>
#include <Windows.h>
#include <fstream>

using namespace std;

//Funktion liest Scoreboard mittels ofstream ein und speichert es in  einen vector von vorzeichenlosen Ganzzahlen
void ausgeben(vector <uint16_t> l)
{
	ofstream f("Scoreboard.txt");
	f << l.at(0);
	f << "\n";
	f << l.at(1);
	f << "\n";
	f << l.at(2);
	f << "\n";
	f << l.at(3);
	f << "\n";
	f << l.at(4);
	f << "\n";
	f << l.at(5);
	f.close();
}

// Klasse für alle Knoepfe im Spiel
class Button
{

public:
	Gosu::Image button_image;
	int16_t Pos_x=0;
	int16_t Pos_y=0;
	int16_t pxl_x=0;
	int16_t pxl_y=0;
	double xM=0;
	double yM=0;

	bool bttn_clicked()
	{
		if (xM >= Pos_x && xM <= Pos_x + pxl_x && yM >= Pos_y && yM <= Pos_y + pxl_y && Gosu::Input::down(Gosu::MS_LEFT))
		{
			return true;
		}
		return false;
	}
};

// Klasse für die Spielfigur
class Spielfigur
{
	Gosu::Image player_image;
	double P_y;

public:

	double P_x = 255;
	bool links = false;
	bool rechts = false;

	Spielfigur(Gosu::Image i, double y) :player_image(i), P_y(y) {}

	void update()
	{
		//Rechts-Links-Bewegung Spielfigur
		if (links && P_x >= 5)
		{
			P_x = P_x - 4;
		}
		if (rechts && P_x <= 485)
		{
			P_x = P_x + 4;
		}
	}

};



class GameWindow : public Gosu::Window
{
	//Alle Image-Dateien
	Gosu::Image background;
	Gosu::Image Rakete;
	Gosu::Image laserbeam;
	Gosu::Image target;
	Gosu::Image GameOver;
	Gosu::Image Restart;
	Gosu::Image L1;
	Gosu::Image L2 = L1;
	Gosu::Image L3 = L1;
	Gosu::Image H1;
	Gosu::Image H2 = H1;
	Gosu::Image H3 = H1;
	Gosu::Image ScoreboardButton;
	Gosu::Image Scoreboard;
	Gosu::Image Close;
	Gosu::Image p1_button;
	Gosu::Image p2_button;
	Gosu::Image p3_button;
	Gosu::Image p4_button;
	Gosu::Image p5_button;
	Gosu::Image p6_button;

	//Vektor als Zwischenspeicher fuer einlesen und schreiben v. Socreboard
	vector <uint16_t> scoreboard{ 0,0,0,0,0,0 };  

	Button P1;
	Button P2;
	Button P3;
	Button P4;
	Button P5;
	Button P6;
	Button reset;
	Button Screbrd;
	Button schliessen;

public:
	//Doubles fuer Positionen
	double y_Laser = 410;
	double y_Target = -10;
	double x_Target = rand() % 509;
	double x_Mouse;
	double y_Mouse;
	double x_Laser;
	double x_Raumschiff = 255;		//x_pos Raumschiff mit Startwert
	//Konstante Hoehe der Spielfigur
	const double y_Raumschiff = 400;

	//Bools v.a. fuer Anzeigen und Schuss
	bool Hit = false;
	bool gameOver = false;
	bool shoot;
	bool show1 = true;
	bool show2 = true;
	bool show3 = true;
	bool reset2 = false;
	bool scb = false;
	bool startscreen = true;
	bool player1 = false;
	bool player2 = false;
	bool player3 = false;
	bool player4 = false;
	bool player5 = false;
	bool player6 = false;
	bool links;
	bool rechts;
	bool values_set=false;
	bool values_set2 = false;
	bool eingelesen = false;
	Spielfigur* Playermodel;


	//Ganzzahlen fuer Zaehler und Score
	uint16_t Score = 0;
	uint16_t gOcount = 0;
	uint16_t Spielerauswahl = 0;

	string numberText;
	string highscoreGlobalText;

	GameWindow()
		: Window(510, 510)
		, background("background.png")
		, Rakete("model.png")
		, laserbeam("laserbeam.png")
		, target("target2.png")
		, GameOver("GameOver.png")
		, Restart("Restart.png")
		, L1("live.png")
		, H1("Herz.png")
		, ScoreboardButton("Scoreboard.png")
		, Scoreboard("background.png")
		, Close("X.png")
		, p1_button("P1_Button.png")
		, p2_button("P2_Button.png")
		, p3_button("P3_Button.png")
		, p4_button("P4_Button.png")
		, p5_button("P5_Button.png")
		, p6_button("P6_Button.png")
	{
		set_caption("Space Invader");
	}


	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		//Font erstellen fuer alle Verwendungen von Schrift
		Gosu::Font font(19,"Impact");

		if (startscreen)
		{
			background.draw(0, 0);
			font.draw_text("Startmenue - Spielstand waehlen", 35, 30, 2, 2.0, 2.0, Gosu::Color::GRAY);
			p1_button.draw(107, 100, 2);
			p2_button.draw(107, 150, 2);
			p3_button.draw(107, 200, 2);
			p4_button.draw(107, 250, 2);
			p5_button.draw(107, 300, 2);
			p6_button.draw(107, 350, 2);
		}
		if (!startscreen)
		{
			if (!shoot) { x_Laser = x_Raumschiff + 9; }
			background.draw(0, 0);
			laserbeam.draw(x_Laser, y_Laser);
			Rakete.draw(x_Raumschiff, y_Raumschiff);
			target.draw(x_Target, y_Target);


			L1.draw(430, 0);
			L2.draw(460, 0);
			L3.draw(490, 0);
			if (show1) { H1.draw(430, 0); }
			if (show2) { H2.draw(460, 0); }
			if (show3) { H3.draw(490, 0); }

			//Score-Anzeige
			numberText = to_string(Score);
			Gosu::Font font(19,"Impact");
			font.draw_text("Score: " + numberText, 0, 0, 0, 1.0, 1.0, Gosu::Color::GRAY);

			if (gameOver)
			{
				show3 = true; show2 = true; show1 = true; gOcount = 0;

				if (!eingelesen)
				{
					ifstream f("Scoreboard.txt");
					char trs;
					uint16_t zws=0;
					f >> zws;
					if (zws == 0) { scoreboard.at(0) = 0; }
					else { scoreboard.at(0) = zws; }
					//f >> trs;
					f >> zws;
					if (zws == 0) { scoreboard.at(1) = 0; }
					else { scoreboard.at(1) = zws; }
					//f >> trs;
					f >> zws;
					if (zws == 0) { scoreboard.at(2) = 0; }
					else { scoreboard.at(2) = zws; }
					//f >> trs;
					f >> zws;
					if (zws == 0) { scoreboard.at(3) = 0; }
					else { scoreboard.at(3) = zws; }
					//f >> trs;
					f >> zws;
					if (zws == 0) { scoreboard.at(4) = 0; }
					else { scoreboard.at(4) = zws; }
					//f >> trs;
					f >> zws;
					if (zws == 0) { scoreboard.at(5) = 0; }
					else { scoreboard.at(5) = zws; }
					f.close();
					eingelesen = true;

				}

				//Highscore Aufzeichnung
				ifstream a("highscore.txt");
				int32_t highscore;
				a >> highscore;
				a.close();
				if (Score > highscore)
				{
					ofstream b("highscore.txt");
					b << Score;
					b.close();
				}
				ifstream c("highscore.txt");
				c >> highscore;
				highscoreGlobalText = to_string(highscore);
				c.close();

				//Spielerspezifischer Highscore 
				if (Score > scoreboard.at(Spielerauswahl))
				{
					scoreboard.at(Spielerauswahl) = Score;
					ausgeben(scoreboard);
				}
				//Array in eine Datei streamen...........................................................

				GameOver.draw(0, 0);
				font.draw_text("Game Over", 200, 250, 0, 1.0, 1.0, Gosu::Color::GRAY);
				font.draw_text("Score:" + numberText, 200, 270, 0, 1.0, 1.0, Gosu::Color::GRAY);
				font.draw_text("System Highscore:" + highscoreGlobalText, 200, 290, 0, 1.0, 1.0, Gosu::Color::GRAY);
				Restart.draw(220, 320);
				ScoreboardButton.draw(216, 350);
				
				if (scb)
				{
					Scoreboard.draw(0, 0);
					Close.draw(490, 0);
					font.draw_text("Scoreboard", 130, 30, 2, 3, 3, Gosu::Color::GRAY);
					font.draw_text("Player 1: " + to_string(scoreboard.at(0)), 10, 100, 2, 1, 1, Gosu::Color::GRAY);
					font.draw_text("Player 2: " + to_string(scoreboard.at(1)), 10, 120, 2, 1, 1, Gosu::Color::GRAY);
					font.draw_text("Player 3: " + to_string(scoreboard.at(2)), 10, 140, 2, 1, 1, Gosu::Color::GRAY);
					font.draw_text("Player 4: " + to_string(scoreboard.at(3)), 10, 160, 2, 1, 1, Gosu::Color::GRAY);
					font.draw_text("Player 5: " + to_string(scoreboard.at(4)), 10, 180, 2, 1, 1, Gosu::Color::GRAY);
					font.draw_text("Player 6: " + to_string(scoreboard.at(5)), 10, 200, 2, 1, 1, Gosu::Color::GRAY);
				}

			}
		}
	}


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//Mauszeigerposition 
		x_Mouse = input().mouse_x();
		y_Mouse = input().mouse_y();

		if (startscreen)	//Startscreen fuer Auswahlen und Presets z.B. Positionen der Tasten
		{
			if (!values_set2) {
				P1.Pos_x = 107;
				P1.Pos_y = 100;
				P1.pxl_x = 58;
				P1.pxl_y = 20;

				P2.Pos_x = 107;
				P2.Pos_y = 150;
				P2.pxl_x = 58;
				P2.pxl_y = 20;

				P3.Pos_x = 107;
				P3.Pos_y = 200;
				P3.pxl_x = 58;
				P3.pxl_y = 20;

				P4.Pos_x = 107;
				P4.Pos_y = 250;
				P4.pxl_x = 58;
				P4.pxl_y = 20;

				P5.Pos_x = 107;
				P5.Pos_y = 300;
				P5.pxl_x = 58;
				P5.pxl_y = 20;

				P6.Pos_x = 107;
				P6.Pos_y = 350;
				P6.pxl_x = 58;
				P6.pxl_y = 20;

				values_set2 = true;
			}

			P1.xM = x_Mouse;
			P1.yM = y_Mouse;
			P2.xM = x_Mouse;
			P2.yM = y_Mouse;
			P3.xM = x_Mouse;
			P3.yM = y_Mouse;
			P4.xM = x_Mouse;
			P4.yM = y_Mouse;
			P5.xM = x_Mouse;
			P5.yM = y_Mouse;
			P6.xM = x_Mouse;
			P6.yM = y_Mouse;


			//Spielfigur erstellen
			Spielfigur s1(Rakete, y_Raumschiff);
			Playermodel = &s1;

			//Spielstand waehlen
			if (P1.bttn_clicked())
			{
				player1 = true;
				startscreen = false;
				Spielerauswahl = 0;				//0 basiert für zugriff auf vector
			}
			else if (P2.bttn_clicked())
			{
				player2 = true;
				startscreen = false;
				Spielerauswahl = 1;
			}
			else if (P3.bttn_clicked())
			{
				player3 = true;
				startscreen = false;
				Spielerauswahl = 2;
			}
			else if (P4.bttn_clicked())
			{
				player4 = true;
				startscreen = false;
				Spielerauswahl = 3;
			}
			else if (P5.bttn_clicked())
			{
				player5 = true;
				startscreen = false;
				Spielerauswahl = 4;
			}
			else if (P6.bttn_clicked())
			{
				player6 = true;
				startscreen = false;
				Spielerauswahl = 5;
			}
		}

		if (!startscreen)	//Spiel laeuft
		{
			if (!values_set)
			{
				reset.Pos_x = 220;
				reset.Pos_y = 320;
				reset.pxl_x = 49;
				reset.pxl_y = 20;

				Screbrd.Pos_x = 216;
				Screbrd.Pos_y = 350;
				Screbrd.pxl_x = 58;
				Screbrd.pxl_y = 20;

				schliessen.Pos_x = 490;
				schliessen.Pos_y = 0;
				schliessen.pxl_x = 20;
				schliessen.pxl_y = 20;
				values_set = true;
			}

			Screbrd.xM = x_Mouse;
			Screbrd.yM = y_Mouse;
			schliessen.xM = x_Mouse;
			schliessen.yM = y_Mouse;
			reset.xM = x_Mouse;
			reset.yM = y_Mouse;

			Playermodel->P_x = x_Raumschiff;
			links = input().down(Gosu::KB_LEFT) && Playermodel->P_x >= 5;
			rechts = input().down(Gosu::KB_RIGHT) && Playermodel->P_x <= 485;
			Playermodel->links = links;
			Playermodel->rechts = rechts;
			Playermodel->update();
			x_Raumschiff = Playermodel->P_x;

			//Zufällig fallendes Target
			//Dropgeschwindigkeit von Score abhängig 
			if (y_Target <= 510)
			{
				if (Score < 8)
				{
					y_Target = y_Target + 1;
				}
				if (Score >= 8 && Score < 15)
				{
					y_Target = y_Target + 2.0;
				}
				if (Score >= 15)
				{
					y_Target = y_Target + 3.0;
				}

				//Hitbox (Target hat 16x16 Pixel)
				if (x_Laser <= x_Target + 16 && x_Laser >= x_Target && y_Laser <= y_Target + 16 && y_Laser >= y_Target - 16)
				{
					Hit = true;
				}
			}

			if (Hit) //Target resetten
			{
				x_Target = rand() % 500;
				y_Target = -10;
				Score++;
				shoot = false;
				Hit = false;
			}
			else if (y_Target >= 390)
			{
				if (gOcount == 0) { show1 = false;	gOcount++; reset2 = true; }
				else if (gOcount == 1) { show2 = false; gOcount++; reset2 = true; }
				else if (gOcount == 2) { gameOver = true; }
			}

			//Schuss, ausgeloest duch KB_SPACE Klick
			if (Gosu::Input::down(Gosu::KB_SPACE)) { shoot = true; }
			if (shoot)
			{
				x_Laser = x_Laser;
				if (y_Laser <= 0)
				{
					shoot = false;
				}
				y_Laser = y_Laser - 7;
			}
			else { y_Laser = 410; }

			//Reset, wenn reset-Button gedrueckt
			if (reset.bttn_clicked() && gameOver)
			{
				gameOver = false;
				eingelesen = false;
				Score = 0;
				y_Target = -10;
			}

			//Scoreboard oeffnen
			if (Screbrd.bttn_clicked() && gameOver)
			{
				scb = true;
			}

			//Scoreboard schliessen
			if (schliessen.bttn_clicked() && gameOver)
			{
				scb = false;
			}

			//Target reset, wenn noch HP vorhanden
			if (reset2 == true)
			{
				x_Target = rand() % 500;
				y_Target = -10;
				reset2 = false;
			}
		}
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
