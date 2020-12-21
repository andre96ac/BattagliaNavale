
// Copyright 2020 Andrea Cuppini

// This file is part of BattagliaNavale.
// BattagliaNavale is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Nome-Programma is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Nome-Programma.  If not, see <http://www.gnu.org/licenses/>.


#include <iostream>
#include <iomanip>
#include <string>
#include<windows.h>


using namespace std;

    

enum DatoTabella: int
{
      naveOrizzontale=10,
      naveVerticale=12,
      naveColpita=20,
      acqua=30,
      acquaColpita=40,
};

enum Verso: int{
    errore=-1,
    orizzontale=10,
    verticale=20,
};

class ConfigParams
{
    public:
        static const int    NMAXCHAR = 100;
        static const int    DIMTABELLA=10;
        static const int    DIMRIQUADRO=5;
        static const int    PRODMODE=true;
        static const int    NNAVI=2;
        static int* GETNAVI(){
            static int ARNAVI[NNAVI]={5,4};
            // static int ARNAVI[NNAVI]={2,2,2,2,3,3,4,4,6};
            return ARNAVI;
        }

};

class Coordinate{
    public:
        int x;
        int y;
        char getParsedX(){
            char _x;
            _x=x+65;
        }
};

class Comando{

    public:
        char* _stringa;

        char*_lettera;
        char*_numero;
        char* _delim=",";

        bool _errore =false;

        Comando(char*comando){
            this->_stringa=comando;
        }
        
        int _countTok(){
            int nVirgole=0;
            for(int i=0; i<strlen(_stringa);i++){
                if (_stringa[i]==','){
                    nVirgole++;
                }
            }
            return nVirgole;
        }
        static char* Tolower(char* stringa){
            char* _stringa;
            _stringa = stringa;
            if (stringa != NULL){
                 for (int i=0; i<strlen(_stringa); i++){
                    _stringa[i]=tolower(_stringa[i]);
                }
            }
            return _stringa;
        }

};


class ComandoSparo: private Comando{
    private:

        int x;
        int y;

        bool _checkLettera(){
            bool error=true;
            if (strlen(this->_lettera)==1){

                char *lettera;
                _lettera[0]=tolower(this->_lettera[0]);
                if (_lettera[0]>='a'&&_lettera[0]<='j'){
                    error=false;
                }

            }
            return error;   
        }

        bool _checkNumero(){
        bool errore = true;
        if (strlen(_numero)==1){
                int numero= (int)_numero[0]-48;
                if ((numero>=0&&numero<ConfigParams::DIMTABELLA)){
                    errore = false;
                }
        }
        return errore;
        }

    public:
        ComandoSparo (char*strComando):Comando(strComando){
            this->_errore=true;
            if (this->_countTok()==1){
                _lettera=strtok(_stringa,_delim);
                _numero=strtok(NULL, _delim);
                if (!this->_checkLettera()&&!this->_checkNumero()){
                    //se non ci sono errori, ricavo i comandi e ritorno tutto ok
                    this->x=(int)_lettera[0]-97;
                    this->y=(int)_numero[0]-48;
                    this->_errore=false;
                }
            }
        }
        int getX(){
        if (!_errore)
            return this->x;
        else
            return Verso::errore;
        }
        int getY(){
            if (!_errore)
                return this->y;
            else
                return Verso::errore;
        }
        bool error(){
            return this->_errore;
        }
};

class ComandoInserimento : private Comando{
private:
    char*_verso;  
    int x;
    int y;
    Verso verso;
    //se la lettera è valida, ritorna false
    bool _checkLettera(){
        bool error=true;
        if (strlen(this->_lettera)==1){

            char *lettera;
            _lettera[0]=tolower(this->_lettera[0]);
            if (_lettera[0]>='a'&&_lettera[0]<='j'){
                error=false;
            }

        }
        return error;   
    }
    bool _checkNumero(){
       bool errore = true;
       if (strlen(_numero)==1){
            int numero= (int)_numero[0]-48;
            if ((numero>=0&&numero<ConfigParams::DIMTABELLA)){
                errore = false;
            }
       }
       return errore;
    }
    bool _checkVerso(){
        bool error=true;
        if (strlen(_verso)==11||strlen(_verso)==9){
            //rendo la stringa minuscola
            for (int i=0; i<strlen(_verso); i++){
                _verso[i]=tolower(_verso[i]);

            }
            if (!strcmp(_verso,"orizzontale")||!strcmp(_verso, "verticale")){
                error=false;
            }
        }
        return error;
    }
public:
    ComandoInserimento(char*comando):Comando(comando){
        this->_errore=true;
        //se ci sono due delimitatori
        if (Comando::_countTok()==2){
            //divido il comando in 3 token
            _lettera=strtok(_stringa,_delim);
            _numero=strtok(NULL, _delim);
            _verso=strtok(NULL, _delim);
            //controllo che i token siano accettabili
            if (!_checkLettera()&&!_checkNumero()&&!_checkVerso()){
                //se non ci sono errori, ricavo i comandi e ritorno tutto ok
                this->x=(int)_lettera[0]-97;
                this->y=(int)_numero[0]-48;
                verso= !strcmp(_verso,"orizzontale")? Verso::orizzontale:Verso::verticale;
                this->_errore=false;
            }

        }
    }
    
    int getX(){
        if (!_errore)
            return this->x;
        else
            return Verso::errore;
    }
    int getY(){
        if (!_errore)
            return this->y;
        else
            return Verso::errore;
    }
    Verso getVerso(){
        if (!_errore){
            return this->verso;
        }
        else{
            return Verso::errore;
        }
        
    }
    bool error(){
        return this->_errore;
    }
};

class Tabella{
    private:
        DatoTabella _dati[ConfigParams::DIMTABELLA][ConfigParams::DIMTABELLA];
    public:

        Tabella(){
            for (int i=0; i<ConfigParams::DIMTABELLA; i++){
                for (int i2=0; i2<ConfigParams::DIMTABELLA; i2++){
                    _dati[i][i2]=DatoTabella::acqua;
                }
            }
        }
        
        void debug(){
            if (!ConfigParams::PRODMODE){
                for (int i=0; i<ConfigParams::DIMTABELLA; i++){
                    for (int i2=0; i2<ConfigParams::DIMTABELLA; i2++){
                        cout<<setw(ConfigParams::DIMRIQUADRO)<<_dati[i][i2];
                    }
                    cout<<endl;
                }
            }
        }
       
        void show(bool hidden){
            cout<<endl;
            cout<<setw(ConfigParams::DIMRIQUADRO+ConfigParams::DIMRIQUADRO/4)<<"";
            //mostro l'intestazione
            for (int i=0; i<ConfigParams::DIMTABELLA; i++){
                cout<<setw(ConfigParams::DIMRIQUADRO)<<i;
            }
            cout<<endl<<endl;

            //mostro i dati
            for (int i=0; i<ConfigParams::DIMTABELLA; i++){
                char carattere=i+65;
                cout<<setw(ConfigParams::DIMRIQUADRO)<<carattere<<setw(ConfigParams::DIMRIQUADRO/4)<<"";
                for (int i2=0; i2<ConfigParams::DIMTABELLA; i2++){
                    cout<<setw(ConfigParams::DIMRIQUADRO)<<getDecodedDato(i,i2, hidden);
                }
                cout<<endl;

            }

        }
        
        bool isFired(int x, int y){
            int error=true;
            if (x>=0&&x<ConfigParams::DIMTABELLA&&y>=0&&y<ConfigParams::DIMTABELLA){
                if (_dati[x][y]==DatoTabella::acqua){
                    _dati[x][y]=DatoTabella::acquaColpita;
                    error=false;
                }
                else if (_dati[x][y]==DatoTabella::naveOrizzontale|_dati[x][y]==DatoTabella::naveVerticale){
                    _dati[x][y]=DatoTabella::naveColpita;
                    error=false;
                }
            }
            return error;
        }

        char getDecodedDato(int x, int y, bool hidden){
            char carReturn;
             switch (_dati[x][y]){
                case DatoTabella::naveOrizzontale:
                    if(hidden)
                        carReturn='O';
                    else
                        carReturn=196; // - 
                break;
                case DatoTabella::naveVerticale:
                    if(hidden)
                        carReturn='O';
                    else
                        carReturn='|';
                break;
                case DatoTabella::acqua:
                    carReturn='O';
                break;
                case DatoTabella::naveColpita:
                    carReturn=197; // †
                break;
                case DatoTabella::acquaColpita:
                    carReturn=157; // ø
                break;
            }
            return carReturn;
        }

        bool inserisciNave(int x, int y, Verso verso, int lunghezza){
            bool errore=false;
            if (x>=ConfigParams::DIMTABELLA|y>=ConfigParams::DIMTABELLA){
                errore=true;
            }
            else{
                if (verso==Verso::verticale){ 
                    if (x+lunghezza>ConfigParams::DIMTABELLA){
                        errore=true;
                    }
                    else{
                        for (int i=0; i<lunghezza; i++){

                            if (this->_dati[x+i][y]==DatoTabella::naveOrizzontale|this->_dati[x+i][y]==DatoTabella::naveVerticale){
                                errore=true;
                                break;
                            }

                        }
                        if (!errore){
                            for (int i=0; i<lunghezza; i++){
                                this->_dati[x+i][y]=DatoTabella::naveVerticale;
                            }
                        }
                    }
                }
                else{
                    if (y+lunghezza>ConfigParams::DIMTABELLA){
                        errore=true;
                    }
                    else{
                        for (int i=0; i<lunghezza; i++){
                            if (this->_dati[x][y+i]==DatoTabella::naveOrizzontale|this->_dati[x][y+i]==DatoTabella::naveVerticale){
                                errore=true;
                                break;
                            }

                        }
                        if (!errore){
                            for (int i=0; i<lunghezza; i++){
                                this->_dati[x][y+i]=DatoTabella::naveOrizzontale;
                            }
                        }
                    }
                }
            }
            return errore;
        }
        bool hasSurvivedShip(){
            bool _has=false;
            bool _hasLost=true;
            for (int i=0; i<ConfigParams::DIMTABELLA;i++){
                for (int i2=0; i2<ConfigParams::DIMTABELLA;i2++){
                    if ((this->_dati[i][i2]==DatoTabella::naveOrizzontale)||(this->_dati[i][i2]==DatoTabella::naveVerticale)){
                        _has=true;
                        break;
                    }
                    if(_has)
                        break;
                }
            }
            return _has;
        }
};


class Giocatore{
    private:
        int id;
        bool cpu;
        Tabella _tabella;
        int *ARNAVI=ConfigParams::GETNAVI();

        bool _initCpu(){
            srand(time(NULL));
            for (int i=0; i<ConfigParams::NNAVI;){
                int x=rand()%ConfigParams::DIMTABELLA;
                int y=rand()%ConfigParams::DIMTABELLA;
                int _direzione = rand()%2;
                Verso verso= _direzione==1? Verso::orizzontale: Verso::verticale;
                if (!ConfigParams::PRODMODE)
                    cout<<"Direzione "<<verso<<endl;
                bool errore=_tabella.inserisciNave(x,y, verso, ARNAVI[i]);
                if (!errore)    
                    i++;
            }
            return false;
        }

        bool _initUser(){
            ComandoInserimento *comando;
            for (int i=0; i<ConfigParams::NNAVI;i++){
                system("cls");
                cout<<"########## GIOCATORE "<<id<<", SCHIERA LE NAVI (es: a,1,verticale) ##########"<<endl<<endl;
                bool errore=true;
                //devo continuare finchè non viene data una stringa valida
                bool errParsing;
                bool errInserting;
                do{ 
                    cout<<"IL TUO SCHIERAMENTO:"<<endl<<endl;
                    _tabella.show(false);
                    cout<<endl;
                    errParsing=true;
                    errInserting=true;

                    char* strRisposta=new char[100];
                    //leggo la risposta
                    cout<<"Nave "<<i+1<<", (lunghezza: "<<ARNAVI[i]<<"): "<<endl<<">";
                    cin>>strRisposta;
                    //con la stringa risposta, creo il comando, e se c'è un errore lo recupero
                    comando= new ComandoInserimento(strRisposta);
                    errParsing=comando->error();
                    //se sono riuscito a fare il parsing della stringa, provo ad inserire la nave
                    if (!errParsing){
                        errInserting=_tabella.inserisciNave(comando->getX(),comando->getY(),comando->getVerso(),ARNAVI[i]);
                    }
                    if (errInserting||errParsing){
                        char*errorMsg;
                        if (errParsing)
                            errorMsg="Attenzione, comando inserito non valido";
                        else
                            errorMsg="Attenzione, non puoi schierare la nave in questa posizone";
                        cout<<errorMsg<<endl;
                        getchar();
                        getchar();
                    }
                    system("cls");

                }
                while(errInserting||errParsing);
            } 
            if (!ConfigParams::PRODMODE){
                cout<<"hai inserito tutte le navi! bravo!"<<endl;
            }
            return false ;
        }

    public:
        Giocatore (int id, bool cpu){
            this->id=id;
            this->cpu=cpu;
        }

        bool init(){
            bool error=true;
            if (cpu)
               error= _initCpu();
            else
                error= _initUser();
            return error;
        };

        void show(bool hidden){
            this->_tabella.show(hidden);
        }

        bool hasLost(){
            if (this->_tabella.hasSurvivedShip()){
                return false;
            }
            else{
                return true;
            }
        }

        bool isFired(int x, int y){
            bool error;
            error= this->_tabella.isFired(x,y);
            return  error;
        }

        Coordinate getCpuShot(){
            //TODO Qui si potrebbe implementare un metodo di calcolo più efficace
            Coordinate coordinate;
            coordinate.x=rand()%ConfigParams::DIMTABELLA;
            coordinate.y=rand()%ConfigParams::DIMTABELLA;
            return coordinate;
        }
        
        void debug(){
            _tabella.debug();
        }
};




class App
{
private:
    bool prodMode;

    char* scelta=new char[ConfigParams::NMAXCHAR];

    void play(bool pvp){
        system("cls");
        //partita 2 giocatori
        if (pvp){
            this->_playPvP();
        }
        else{
            this->_playPvCpu();
        }

        getchar();
    }

    void _playPvP(){
            Giocatore giocatore1(1, false);
            Giocatore giocatore2(2, false);
            giocatore1.init();
            system("cls");
            cout<<endl<<"GIOCATORE 1, lascia i comandi al GIOCATORE 2 "<<endl<<endl<<"GIOCATORE 2, quando sei pronto premi un tasto..."<<endl;
            getchar();
            getchar();
            system("cls");
            giocatore2.init();
            cout<<"Tutte le navi sono state schierate"<<endl;
            char* str;
            do{
                bool errParsing;
                bool errShot;
            
                do{
                    errParsing=true;
                    errShot=true;
                    system("cls");
                    cout<<"Tocca al giocatore 1... "<<endl;
                    getchar();
                    getchar();
                    system("cls");
                    cout<<"# LE TUE NAVI:"<<endl<<endl;
                    giocatore1.show(false);
                    cout<<endl<<"# LE NAVI DEL TUO AVVERSARIO:"<<endl<<endl;
                    giocatore2.show(true);
                    cout<<endl<<endl<<"Coordinate di fuoco: (Esempio: \"a,3\")"<<endl<<">";
                    str=new char[ConfigParams::NMAXCHAR];
                    cin>>str;
                    ComandoSparo comando (str);
                    errParsing=comando.error();
                    if (!errParsing){
                        errShot=giocatore2.isFired(comando.getX(),comando.getY());
                    }
                    delete(str);
                    if (errParsing){
                        cout<<"Attenzione, comando inserito non valido"<<endl;
                        getchar();
                        getchar();
                    }
                    else if (errShot){
                        cout<<"Attenzione, non puoi sparare qui"<<endl;
                        getchar();
                        getchar();
                    }
                }while(errParsing||errShot);
                if (!giocatore2.hasLost()){
                        do{
                        errParsing=true;
                        errShot=true;
                        system("cls");
                        cout<<"Tocca al giocatore 2... "<<endl;
                        getchar();
                        getchar();
                        system("cls");
                        cout<<"# LE TUE NAVI:"<<endl<<endl;
                        giocatore2.show(false);
                        cout<<endl<<"# LE NAVI DEL TUO AVVERSARIO:"<<endl<<endl;
                        giocatore1.show(true);
                        cout<<endl<<endl<<"Coordinate di fuoco: (Esempio: \"a,3\")"<<endl<<">";
                        str=new char[ConfigParams::NMAXCHAR];
                        cin>>str;
                        ComandoSparo comando (str);
                        errParsing=comando.error();
                        if (!errParsing){
                            errShot=giocatore1.isFired(comando.getX(),comando.getY());
                        }
                        delete(str);
                        if (errParsing){
                            cout<<"Attenzione, comando inserito non valido"<<endl;
                            getchar();
                            getchar();
                        }
                        else if (errShot){
                            cout<<"Attenzione, non puoi sparare qui"<<endl;
                            getchar();
                            getchar();
                        }
                    }while(errParsing||errShot);
                }
            }while(!giocatore1.hasLost()&&!giocatore2.hasLost());
            system("cls");
            cout<<"Partita terminata!";
            getchar();
            getchar();
            system("cls");
            cout<<"SCHIERAMENTO DEL GIOCATORE 1:"<<endl;
            giocatore1.show(false);
            cout<<endl<<endl<<"SCHIERAMENTO DEL GIOCATORE 2:"<<endl;
            giocatore2.show(false);
            cout<<endl<<endl;
            if (giocatore1.hasLost())
                cout<<"############################## COMPLIMENTI GIOCATORE 2 HAI VINTO! ##############################"<<endl;
            else
                cout<<"############################## COMPLIMENTI GIOCATORE 1 HAI VINTO! ##############################"<<endl;
    }
    
    void _playPvCpu(){
        Giocatore giocatore1(1,false);
        Giocatore giocatore2(2,true);
        giocatore1.init();
        giocatore2.init();
        char* str;
        do
        {
            bool errParsing;
            bool errShot;
            do{
                errParsing=true;
                errShot=true;
                system("cls");
                cout<<"################# Tocca a te..."<<endl<<endl<<endl;
                giocatore2.debug();
                cout<<"# LE TUE NAVI:"<<endl<<endl;
                    giocatore1.show(false);
                    cout<<endl<<"# LE NAVI DEL TUO AVVERSARIO:"<<endl<<endl;
                    giocatore2.show(true);
                    cout<<endl<<endl<<"Coordinate di fuoco: (Esempio: \"a,3\")"<<endl<<">";
                    str=new char[ConfigParams::NMAXCHAR];
                    cin>>str;
                    ComandoSparo comando (str);
                    errParsing=comando.error();
                    if (!errParsing){
                        errShot=giocatore2.isFired(comando.getX(),comando.getY());
                    }
                    delete(str);
                    if (errParsing){
                        cout<<"Attenzione, comando inserito non valido"<<endl;
                        getchar();
                        getchar();
                    }
                    else if (errShot){
                        cout<<"Attenzione, non puoi sparare qui"<<endl;
                        getchar();
                        getchar();
                    }

            } while (errParsing||errShot);
            if (!giocatore2.hasLost()){
                int x;
                int y;
                errShot=true;
                system("cls");
                cout<<"################# Tocca a me..."<<endl<<endl;
                getchar();
                getchar();
                do
                {
                    Coordinate sparo;
                    sparo=giocatore2.getCpuShot();
                    errShot=giocatore1.isFired(sparo.x,sparo.y);
                    if (!errShot){
                        cout<<">"<<sparo.getParsedX()<<" "<<sparo.y;
                        getchar();
                    }
                } while (errShot);
                
            }
        } while (!giocatore1.hasLost()&&!giocatore2.hasLost());
        system("cls");
        cout<<"Partita terminata!";
        getchar();
        system("cls");
        cout<<"IL TUO SCHIERAMENTO:"<<endl;
        giocatore1.show(false);
        cout<<endl<<endl<<"LO SCHIERAMENTO AVVERSARIO:"<<endl;
        giocatore2.show(false);
        cout<<endl<<endl;
        if (giocatore1.hasLost())
            cout<<"############################## ANDRA' MEGLIO LA PROSSIMA VOLTA :( ##############################"<<endl;
        else
            cout<<"############################## COMPLIMENTI, HAI VINTO! ##############################"<<endl;
        
    }
    void showHeader(){
        cout<<"###################################################################################################"<<endl;
        cout<<"############################## BENVENUTO NELLA BATTAGLIA NAVALE 1.0! ##############################"<<endl;
        cout<<"###################################################################################################"<<endl;
        cout<<endl;
    }

    void showMenu(){
        system ("cls");
        this->showHeader();
        cout<<"Cosa vuoi fare?"<<endl<<endl<<"> PvP"<<endl<<"> PvCPU"<<endl<<"> ESCI"<<endl<<endl<<">";
        cin>>this->scelta;
    }

public:
    
    App(){
    }

    //Questa è l'esecuzione
    bool exec(){
        bool error= false;
        do{
        if (!ConfigParams::PRODMODE)
            cout<<"endtro nell'init..."<<endl;
        //mostro il menu e leggo la scelta
        this->showMenu();
        //scelta "player vs player"

        if (!(strcmp((Comando::Tolower(this->scelta)),"pvp"))){

            this->play(true);

        }
        
        //scelta "player vs cpu"
        else if (!(strcmp((Comando::Tolower(this->scelta)),"pvcpu"))){
            this->play(false);
        }

        //scelta "esci"
        else if (!strcmp((Comando::Tolower(this->scelta)),"esci")){
            cout<<"Alla prossima! "<<endl<<"Premi un tasto per uscire..."<<endl;
        }

        //altra scelta
        else{
            cout<<"Opzione non valida :("<<endl<<endl<<"premi un tasto..."<<endl;
            getchar();
            getchar();
        }


        }while (strcmp((Comando::Tolower(this->scelta)),"esci"));
        return error;
    }

    

};



int main(){

    App app;
    return app.exec();
}
