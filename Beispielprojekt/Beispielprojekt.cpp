#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <dos.h>
#include <Windows.h>
#include <fstream>

using namespace std;

// Klasse für alle Knoepfe im Spiel
class Button
{
	Gosu::Image button_image;
	int16_t Pos_x;
	int16_t Pos_y;
	int16_t pxl_x;
	int16_t pxl_y;
	double xM;
	double yM;

public:
	Button(Gosu::Image i, int16_t x, int16_t y, int16_t px, int16_t py, double xm, double ym) :button_image(i), Pos_x(x), Pos_y(y), pxl_x(px), pxl_y(py), xM(xm), yM(ym) {}
	
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
	
	double P_x=255;
	bool links=false;
	bool rechts=false;

	Spielfigur(Gosu::Image i,double y):player_image(i),P_y(y){}

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

public:
	//Doubles fuer Positionen
	double y_Laser = 410;
	double y_Target = -10;
	double x_Target = rand() % 509;
	double x_Mouse;
	double y_Mouse;
	double x_Laser;
	double x_Raumschiff = 255;		//x_pos Raumschiff mit Startwert
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
	bool startscreen = false; //////true
	bool player1 = false;
	bool player2 = false;
	bool player3 = false;
	bool player4 = false;
	bool player5 = false;
	bool player6 = false;
	bool links;
	bool rechts;


	//Ganzzahlen fuer Zaehler und Score
	uint16_t Score = 0;
	uint16_t gOcount = 0;

	vector <uint16_t> scoreboard;  //evtl. ueber eine Datei mit lesen und schreiben ueber Vector

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
		Gosu::Font font(19);

		if (startscreen)
		{
			background.draw(0, 0);
			font.draw_text("Spielstand waehlen", 105, 30, 2, 2.0, 2.0, Gosu::Color::GRAY);
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
			Gosu::Font font(19);
			font.draw_text("Score: " + numberText, 0, 0, 0, 1.0, 1.0, Gosu::Color::GRAY);

			if (gameOver)
			{
				show3 = true; show2 = true; show1 = true; gOcount = 0;
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

				GameOver.draw(0, 0);
				font.draw_text("Game Over", 200, 250, 0, 1.0, 1.0, Gosu::Color::GRAY);
				font.draw_text("Score:" + numberText, 200, 270, 0, 1.0, 1.0, Gosu::Color::GRAY);
				font.draw_text("System Highscore:" + highscoreGlobalText, 200, 290, 0, 1.0, 1.0, Gosu::Color::GRAY);
				Restart.draw(220, 320);
				ScoreboardButton.draw(216, 350);

			}
			if (scb)
			{
				Scoreboard.draw(0, 0);
				Close.draw(490, 0);
				font.draw_text("Scoreboard", 130, 30, 2, 3, 3, Gosu::Color::GRAY);
				font.draw_text("Player 1:", 10, 100, 2, 1, 1, Gosu::Color::GRAY);
				font.draw_text("Player 2:", 10, 120, 2, 1, 1, Gosu::Color::GRAY);
				font.draw_text("Player 3:", 10, 140, 2, 1, 1, Gosu::Color::GRAY);
				font.draw_text("Player 4:", 10, 160, 2, 1, 1, Gosu::Color::GRAY);
				font.draw_text("Player 5:", 10, 180, 2, 1, 1, Gosu::Color::GRAY);
				font.draw_text("Player 6:", 10, 200, 2, 1, 1, Gosu::Color::GRAY);
			}
		}
	}


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//Buttons
		Button reset(Restart, 220, 320, 49, 20, x_Mouse, y_Mouse);
		Button Screbrd(ScoreboardButton, 216, 350, 58, 20, x_Mouse, y_Mouse);
		Button schliessen(Close,490,0,20,20,x_Mouse,y_Mouse);

		//Spielfigur erstellen
		Spielfigur s1(Rakete, y_Raumschiff);
		s1.P_x = x_Raumschiff;
		links = input().down(Gosu::KB_LEFT) && s1.P_x >= 5;
		rechts = input().down(Gosu::KB_RIGHT) && s1.P_x <= 485;
		s1.links = links;
		s1.rechts = rechts;
		s1.update();
		x_Raumschiff = s1.P_x;

		if(startscreen)
		{
			//Hitboxen fuer P1-,P2-,... Tasten & in Abhaengigkeit richtige bools setzen
		}
		if (!startscreen)
		{
			x_Mouse = input().mouse_x();
			y_Mouse = input().mouse_y();

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
					y_Target = y_Target + 2.5;
				}
				if (Score >= 15)
				{
					y_Target = y_Target + 3.5;
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
			else if (y_Target >= 410)
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


/*
To-Do
	1) Screen bei oeffnen des Spiels zum auswaehlen von einem der 6 "Player" (erledigt) -> Auswahl speichern
	2) Fuer jeden Spieler extra .txt und in die jeweilige je nach vorheiger Wahl den Highscore speichern (System Highscore bleibt zusaetzlich erhalten)
	3) Scores aus allen .txts auslesen und bei Aufruf in Scoreboard anzeigen
	4) 255 in Spielfigur Klasse setzen anstatt in GameWindow
	5) evtl. Audiofeedback bei Treffer
*/