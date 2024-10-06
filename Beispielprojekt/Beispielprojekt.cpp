#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <dos.h>
#include <Windows.h>
#include <fstream>

using namespace std;

class GameWindow : public Gosu::Window
{
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

public:
	//Doubles fuer Positionen
	double x_Rakete = 255;
	double y_Laser = 410;
	double y_Target = -10;
	double x_Target = rand() % 509;
	double x_Mouse;
	double y_Mouse;
	double x_Laser;

	//Bools v.a. fuer Anzeigen und Schuss
	bool Hit = false;
	bool gameOver = false;
	bool shoot;
	bool show1 = true;
	bool show2 = true;
	bool show3 = true;
	bool reset2 = false;
	bool scb = false;

	//Ganzzahlen fuer Zaehler und Score
	uint16_t Score = 0;
	uint16_t gOcount = 0;


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
	{
		set_caption("Space Invader");
	}

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		if (!shoot) { x_Laser = x_Rakete + 9; }
		background.draw(0, 0);
		laserbeam.draw(x_Laser, y_Laser);
		Rakete.draw(x_Rakete, 400);
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


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		x_Mouse = input().mouse_x();
		y_Mouse = input().mouse_y();

		//Rechts-Links-Bewegung Spielfigur
		if (input().down(Gosu::KB_LEFT) && x_Rakete >= 5)
		{
			x_Rakete = x_Rakete - 4;
		}
		if (input().down(Gosu::KB_RIGHT) && x_Rakete <= 485)
		{
			x_Rakete = x_Rakete + 4;
		}


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
		if (x_Mouse >= 220 && x_Mouse <= 270 && y_Mouse >= 320 && y_Mouse <= 340 && Gosu::Input::down(Gosu::MS_LEFT) && gameOver)
		{
			gameOver = false;
			Score = 0;
			y_Target = -10;
		}

		//Scoreboard oeffnen
		if (x_Mouse >= 216 && x_Mouse <= 274 && y_Mouse >= 350 && y_Mouse <= 370 && Gosu::Input::down(Gosu::MS_LEFT) && gameOver)
		{
			scb = true;
		}

		//Scoreboard schliessen
		if (x_Mouse >= 490 && x_Mouse <= 510 && y_Mouse >= 0 && y_Mouse <= 20 && Gosu::Input::down(Gosu::MS_LEFT) && gameOver)
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

};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}


/*
To-Do
	1) Screen bei oeffnen des Spiels zum auswaehlen von einem der 6 "Player" -> auswahl speichern
	2) Ueber bool, der nach Schritt 1) veraendert wird, komplettes restliches Programm in der GameWindow Klasse starten
	3) Fuer jeden Spieler extra .txt und in die jeweilige je nach vorheiger Wahl den Highscore speichern (System Highsore bleibt extra erhalten
	4) Scores aus allen .txts auslesen und bei Aufruf in Scoreboard anzeigen
	5) evtl. Audiofeedback bei Treffer
*/