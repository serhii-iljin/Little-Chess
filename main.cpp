#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
//#include "CONNECTOR.h"

using namespace sf;
using namespace std;

void menu(), Freeplay(), Engine();

void Engine()
{
    //Window rendering, creating sprites
    RenderWindow window(VideoMode(1520, 1520), "ENGINE", sf::Style::Close);
    Image icon;
    icon.loadFromFile("images/icon2.png"); window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Texture T_Pieces, T_Board; T_Pieces.loadFromFile("images/199.png"); T_Board.loadFromFile("images/board4.png");
    Sprite sPieces[32]; for(auto &sPiece : sPieces) sPiece.setTexture(T_Pieces);
    Sprite sBoard(T_Board);

    Texture goB;
    goB.loadFromFile("images/knight.png");
    Sprite SGoB(goB);
    SGoB.setPosition(100,100);
    //Const that describes size of piece
    const int SIZE = 190;
    //Describing game elements
    vector<vector<int> > vecBoard =
            {{-1,-2,-3,-4,-5,-3,-2,-1},
             {-6,-6,-6,-6,-6,-6,-6,-6},
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0},
             {6, 6, 6, 6, 6, 6, 6, 6},
             {1, 2, 3, 4, 5, 3, 2, 1}};
    bool isMove = false;
    float dx = 0, dy = 0;
    int chosen = 0;
    Vector2f oldPos, newPos;
    string notes, pos = "";
    //
    //  FUNCTION AREA BEGINNING
    //
    function<Vector2f(char, char)> getCoord = [](char a, char b){return Vector2f((int(a)-97) * SIZE, (56 - int(b)) * SIZE);};
    function<void(string)> move = [&](string note)
    {
        Vector2f oldPos = getCoord(note[0], note[1]);
        Vector2f newPos = getCoord(note[2], note[3]);
        for(auto &sPiece : sPieces) if(sPiece.getPosition() == newPos) sPiece.setPosition(-200, 0);
        for(auto &sPiece : sPieces) if(sPiece.getPosition() == oldPos) sPiece.setPosition(newPos);
        if (note == "e1g1") move("h1f1");
        else if (note == "e8g8") move("h8f8");
        else if (note == "e1c1") move("a1d1");
        else if (note == "e8c8") move("a8d8");
    };
    function<void(void)> loadPosition = [&]()
    {
        for(int i = 0, k = 0; i < 8; i++)
        {
            for(int j = 0; j < 8 && vecBoard[i][j]; j++, k++)
            {
                sPieces[k].setTextureRect(IntRect(SIZE * (abs(vecBoard[i][j]) - 1), SIZE * (vecBoard[i][j] > 0 ? 1 : 0), SIZE, SIZE));
                sPieces[k].setPosition(float(SIZE * j), float(SIZE * i));
            }
        }
        for(int i = 0; i < pos.length(); i += 5) {cout << pos.substr(i, 4)  << " "; move(pos.substr(i, 4));}
        cout << endl;
    };
    function<string(Vector2f)> getChessNote = [](Vector2f pos)
    {
        string s;
        s.push_back(char(pos.x / SIZE + 97)); s.push_back(char(56 - pos.y / SIZE));
        return s;
    };
    //
    //  FUNCTION AREA END
    //
    loadPosition();
    while(window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Event e{};
        while(window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::BackSpace) pos.erase(max(int(pos.length() - 6), 0), 5), loadPosition();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {window.close(); menu();}
            for (int i = 0; i < 32; i++)
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && sPieces[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                {isMove = true, chosen = i, dx = mousePos.x - sPieces[i].getPosition().x, dy = mousePos.y - sPieces[i].getPosition().y, oldPos = sPieces[i].getPosition();}

            if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Left || Keyboard::isKeyPressed(Keyboard::Space))
            {
                isMove = false;
                Vector2f calibrate = sPieces[chosen].getPosition() + Vector2f(int(SIZE / 2), int(SIZE / 2));
                newPos = Vector2f(SIZE * int(calibrate.x / SIZE), SIZE * int(calibrate.y / SIZE));
                notes = getChessNote(oldPos) + getChessNote(newPos);
                if (!(notes[0] == notes[2] && notes[1] == notes[3])) pos += notes + " ", move(notes);
                sPieces[chosen].setPosition(newPos);
                cout << pos << endl;
            }
            if(Keyboard::isKeyPressed(Keyboard::Space))
            {
                notes = "e2e4";
                oldPos = getCoord(notes[0], notes[1]);
                newPos = getCoord(notes[2], notes[3]);
                for(int i = 0; i < 32; i++) if (sPieces[i].getPosition()==oldPos) chosen=i;
                for(int k = 0; k < 10000; k++)
                {
                    Vector2f p = newPos - oldPos;
                    sPieces[chosen].move(max(p.x/1000000000, newPos.x), max(p.x/100000000, newPos.y));
                }
                move(notes);
                sPieces[chosen].setPosition(newPos);
            }
        }
        if(isMove) sPieces[chosen].setPosition(mousePos.x - dx, mousePos.y - dy);
        window.clear();
        window.draw(sBoard);
        for(const auto &sPiece : sPieces) window.draw(sPiece);
        window.display();
    }
}

void Freeplay()
{
    //Window rendering, creating sprites
    RenderWindow window(VideoMode(1520, 1520), "FREEPLAY", sf::Style::Close);
    Image icon;
    icon.loadFromFile("images/icon2.png"); window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Texture T_Pieces, T_Board; T_Pieces.loadFromFile("images/199.png"); T_Board.loadFromFile("images/board4.png");
    Sprite sPieces[32]; for(auto &sPiece : sPieces) sPiece.setTexture(T_Pieces);
    Sprite sBoard(T_Board);

    Texture goB;
    goB.loadFromFile("images/knight.png");
    Sprite SGoB(goB);
    SGoB.setPosition(100,100);
    //Const that describes size of piece
    const int SIZE = 190;
    //Describing game elements
    vector<vector<int> > vecBoard =
    {{-1,-2,-3,-4,-5,-3,-2,-1},
    {-6,-6,-6,-6,-6,-6,-6,-6},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {6, 6, 6, 6, 6, 6, 6, 6},
    {1, 2, 3, 4, 5, 3, 2, 1}};
    bool isMove = false;
    float dx = 0, dy = 0;
    int chosen = 0;
    Vector2f oldPos, newPos;
    string notes, pos = "";
    string playerInput;
    string playerText;
    int posT = 0;
    //
    //  FUNCTION AREA BEGINNING
    //
    function<Vector2f(char, char)> getCoord = [](char a, char b){return Vector2f((int(a)-97) * SIZE, (56 - int(b)) * SIZE);};
    function<void(string)> move = [&](string note)
    {
        Vector2f oldPos = getCoord(note[0], note[1]);
        Vector2f newPos = getCoord(note[2], note[3]);
        for(auto &sPiece : sPieces) if(sPiece.getPosition() == newPos) sPiece.setPosition(-200, 0);
        for(auto &sPiece : sPieces) if(sPiece.getPosition() == oldPos) sPiece.setPosition(newPos);
        if (note == "e1g1") move("h1f1");
        else if (note == "e8g8") move("h8f8");
        else if (note == "e1c1") move("a1d1");
        else if (note == "e8c8") move("a8d8");
    };
    function<void(void)> loadPosition = [&]()
    {
        for(int i = 0, k = 0; i < 8; i++)
        {
            for(int j = 0; j < 8 && vecBoard[i][j]; j++, k++)
            {
                sPieces[k].setTextureRect(IntRect(SIZE * (abs(vecBoard[i][j]) - 1), SIZE * (vecBoard[i][j] > 0 ? 1 : 0), SIZE, SIZE));
                sPieces[k].setPosition(float(SIZE * j), float(SIZE * i));
            }
        }
        for(int i = 0; i < pos.length(); i += 5) {cout << pos.substr(i, 4)  << " "; move(pos.substr(i, 4));}
        cout << endl;
    };
    function<string(Vector2f)> getChessNote = [](Vector2f pos)
    {
        string s;
        s.push_back(char(pos.x / SIZE + 97)); s.push_back(char(56 - pos.y / SIZE));
        return s;
    };
    //
    //  FUNCTION AREA END
    //
    loadPosition();
    while(window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Event e{};
        while(window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::BackSpace) pos.erase(max(int(pos.length() - 6), 0), 5), loadPosition();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {window.close(); menu();}
            for (int i = 0; i < 32; i++)
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && sPieces[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
            {isMove = true, chosen = i, dx = mousePos.x - sPieces[i].getPosition().x, dy = mousePos.y - sPieces[i].getPosition().y, oldPos = sPieces[i].getPosition();}

            if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Left)
            {
                isMove = false;
                Vector2f calibrate = sPieces[chosen].getPosition() + Vector2f(int(SIZE / 2), int(SIZE / 2));
                newPos = Vector2f(SIZE * int(calibrate.x / SIZE), SIZE * int(calibrate.y / SIZE));
                notes = getChessNote(oldPos) + getChessNote(newPos);
                if (!(notes[0] == notes[2] && notes[1] == notes[3])) pos += notes + " ", move(notes);
                sPieces[chosen].setPosition(newPos);
                cout << pos << endl;
            }
            if (e.type == sf::Event::TextEntered && e.text.unicode != 8)
            {
                playerInput += e.text.unicode;
                playerText = playerInput;
            }
        }
        if(playerText.size()%4 == 0 && playerText.size() != 0)
        {
            cout << playerText << endl;
            notes = "";
            for(int i = 4*posT; i < playerText.size(); i++) notes+=playerText[i];
            pos += notes + " ";
            posT++;
            cout << notes << endl;
            playerText.clear();
            oldPos = getCoord(notes[0], notes[1]);
            newPos = getCoord(notes[2], notes[3]);
            for(int i = 0; i < 32; i++) if (sPieces[i].getPosition()==oldPos) chosen=i;
            for(int k = 0; k < 10000; k++)
            {
                Vector2f p = newPos - oldPos;
                sPieces[chosen].move(max(p.x/1000000000, newPos.x), max(p.x/100000000, newPos.y));
            }
            move(notes);
            sPieces[chosen].setPosition(newPos);
        }
        if(isMove) sPieces[chosen].setPosition(mousePos.x - dx, mousePos.y - dy);
        window.clear();
        window.draw(sBoard);
        for(const auto &sPiece : sPieces) window.draw(sPiece);
        window.display();
    }
}

void menu()
{
    //prevsize 2000/1520
    RenderWindow windowAll(VideoMode(1920, 1520), "Little Chess!", sf::Style::Close);
    Texture startFree;
    Texture back;
    Texture startEng;
    startFree.loadFromFile("images/buttons/b3.png");
    back.loadFromFile("images/back.png");
    startEng.loadFromFile("images/buttons/b2.png");
    Sprite Sback(back);
    Sprite SStartEng(startEng);
    Sprite SStartFree(startFree);
    SStartFree.setPosition(450, 170);
    SStartEng.setPosition(450, -60);
    while(windowAll.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(windowAll);
        Event e{};
        while(windowAll.pollEvent(e))
        {
            if (e.type == Event::Closed) windowAll.close();
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && SStartFree.getGlobalBounds().contains(mousePos.x, mousePos.y) && !SStartEng.getGlobalBounds().contains(mousePos.x, mousePos.y)) {windowAll.close(); Freeplay();}
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && SStartEng.getGlobalBounds().contains(mousePos.x, mousePos.y)) {windowAll.close(); Engine();}
        }
        windowAll.clear();
        windowAll.draw(SStartFree);
        windowAll.draw(SStartEng);
        windowAll.draw(Sback);
        windowAll.display();
    }
}

int main()
{
    menu();
    return 0;
}