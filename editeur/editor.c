// C Source File
// Created 04/07/2002; 16:27:56


#define USE_TI89              // Compile for TI-89
//#define USE_TI92PLUS          // Compile for TI-92 Plus
//#define USE_V200              // Compile for V200

#define ENABLE_ERROR_RETURN   // Enable Returning Errors to AMS

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#ifdef USE_TI89

  #define PRESS_ESCAPE (_rowread(~0b1000000) & 0b00000001)

  #define PRESS_SHIFT  (_rowread(~0b0000001) & 0b00100000)

  #define PRESS_SECOND (_rowread(~0b0000001) & 0b00010000)

  #define PRESS_RIGHT  (_rowread(~0b0000001) & 0b00001000)

  #define PRESS_DOWN   (_rowread(~0b0000001) & 0b00000100)

  #define PRESS_LEFT   (_rowread(~0b0000001) & 0b00000010)

  #define PRESS_UP     (_rowread(~0b0000001) & 0b00000001)
  
  #define PRESS_ENTER		(_rowread(0xfffd) & 1)

	#define MAX_X 20
	#define MAX_Y 11

#else

  #define PRESS_ESCAPE (_rowread(~0b0100000000) & 0b01000000)

  #define PRESS_SECOND   (_rowread(~0b0000000001) & 0b00001000) // touche 'hand'

  #define PRESS_SHIFT  (_rowread(~0b0000000001) & 0b00000100)

  //#define PRESS_SECOND (GX_rowread(~0b0000000001) & 0b00000001)

  #define PRESS_RIGHT  (_rowread(~0b0000000001) & 0b01000000)

  #define PRESS_DOWN   (_rowread(~0b0000000001) & 0b10000000)

  #define PRESS_LEFT   (_rowread(~0b0000000001) & 0b00010000)

  #define PRESS_UP        (_rowread(~0b0000000001) & 0b00100000)
  
  #define PRESS_ENTER		(_rowread(0xfdff) & 2)
  
  #define MAX_X 31
	#define MAX_Y 15
  
#endif


void *Image_C0 ;
void *Image_C1 ;

short taille_case = 6 ;

// case noire
BITMAP case_n_C0 = {7,7,{0xFE,0xFE,0xC2,0xC2,0xC2,0xC2,0xFE}};
BITMAP case_n_C1 = {7,7,{0x00,0x7C,0x7C,0x7C,0x7C,0x7C,0x00}};

// case blanche
BITMAP case_b_C0 = {7,7,{0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE}};
BITMAP case_n_d_C1 = {7,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

// curseur
//BITMAP curs_C0 = {7,7,{0xFE,0x82,0x82,0x82,0x82,0x82,0xFE}};
BITMAP curs_C1 = {7,7,{0xFE,0x82,0x82,0x82,0x82,0x82,0xFE}};

void InputStr (char *buffer, short maxlen)
{
  SCR_STATE ss;
  short key, i = 0;
  buffer[0] = 0;
  SaveScrState (&ss);
  do
    {
      MoveTo (ss.CurX, ss.CurY);
      printf ("%s_  ", buffer);
        // Deux espaces sont nécessaires pour une fonte  F_4x6
      key = ngetchx ();
      if (key >= ' ' && key <= '~' && i < maxlen) buffer[i++] = key;
      if (key == KEY_BACKSPACE && i) i--;
      buffer[i] = 0;
    } while (key != KEY_ENTER);
}


struct niveau
{
	unsigned short id ;	
	unsigned char taille_x ;	
	unsigned char taille_y ;
	unsigned char nb_max_col ;
	unsigned char nb_max_ligne ;
	
	BOOL mat[MAX_Y][MAX_X]	;
	short img_mario_x ;
	short img_mario_y ;
} ;

struct fichier
{
	char nom[21] ; // nom des niveaux
	char auteur[16] ; // auteur
	unsigned short nb_levels ; // le nombre de niveaux contenus dans le fichier
	unsigned short (*tab_taille_x_y)[2] ; // les tableaux contenant les taille x et y des niveaux
	char *level ; // les niveaux
} ;

void afficher_sprite(short couleur,short x,short y)
{
	if(!couleur)  // case dévoilée noire
	{
		Image_C0 = &case_n_C0;
		Image_C1 = &case_n_C1;
		
		SetPlane(LIGHT_PLANE) ;
		BitmapPut (x,y,Image_C0,ScrRect,A_REPLACE);
		SetPlane(DARK_PLANE) ;
		BitmapPut (x,y,Image_C1,ScrRect,A_REPLACE);
	}

	if(couleur==1)  // case dévoilée blanche
	{
		Image_C0 = &case_b_C0;
		Image_C1 = &case_n_d_C1;
		SetPlane(LIGHT_PLANE) ;
		BitmapPut (x,y,Image_C0,ScrRect,A_REPLACE);
		SetPlane(DARK_PLANE) ;
		BitmapPut (x,y,Image_C1,ScrRect,A_REPLACE);
	}
	
	if(couleur==2)  // curseur
	{
		Image_C1 = &curs_C1;
		SetPlane(DARK_PLANE) ;
		BitmapPut (x,y,Image_C1,ScrRect,A_XOR);
	}
}

	void _main(void)
	{
			struct fichier fic ;
			struct niveau niv ;
			FILE * f ;	
			long pos_fic,pos_xy ;
			unsigned char tmp ;
			unsigned short level ;
			unsigned short cpt,temp ;
			unsigned short tx,ty ;
			
			HANDLE handle ;
			short result ;
			
			INT_HANDLER save_int_1 = GetIntVec (AUTO_INT_1);

			//clrscr() ;
			
			// menu

		  handle = PopupNew ("Picross editor 1.0", 50);
		  PopupAddText (handle,0, "nouveau fichier", 1);
		  PopupAddText (handle,0, "charger fichier", 2);
		  result = PopupDo (handle, CENTER, CENTER, 0);
		  HeapFree (handle);

	if(result == 1)
	{
			// création d'un nouveau niveau
			
  		handle = DialogNewSimple (130, 45);
 		  char buffer[21] = "" ;
  		DialogAddTitle (handle, "nom du fichier", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "(8) ",0,8,8);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL)	;	
			HeapFree (handle);
			
			f = fopen(buffer,"w+") ;
			
			strcpy(buffer,"") ;
			handle = DialogNewSimple (130, 45); 		
  		DialogAddTitle (handle, "nom des niveaux", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "(20) ",0,20,10);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL) ;
			HeapFree (handle);
	
			fputs("_picross_lvl_",f) ; // signature du fichier
			tmp = (unsigned char)strlen(buffer) ; // taille du nom de fichier
			fputs(buffer,f) ; // on met le nom du fichier
			//fputs("\r",f) ;
			for(cpt=tmp+1;cpt<=20;cpt++) // on complete pour avoir 20 char
			{
					fputs(" ",f) ;
			} ;
			
			// auteur
			strcpy(buffer,"") ;
			handle = DialogNewSimple (130, 45); 		
  		DialogAddTitle (handle, "votre nom", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "(15) ",0,15,10);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL) ;
			HeapFree (handle);

			tmp = (unsigned char)strlen(buffer) ; // taille du nom de l'auteur
			fputs(buffer,f) ; // on met le nom de l'auteur
			//fputs("\r",f) ;
			for(cpt=tmp+1;cpt<=15;cpt++) // on complete pour avoir 20 char
			{
					fputs(" ",f) ;
			} ;
						
			strcpy(buffer,"") ;
			handle = DialogNewSimple (130, 45); 		
  		DialogAddTitle (handle, "nombre de levels du fichier", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "",0,2,2);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL) ;
			HeapFree (handle);

			tmp = atoi(buffer) ;
			if(tmp<10) fputs("0",f) ;
			fputs(buffer,f) ; // nombre de levels du fichier

			temp = tmp ;
			
			strcpy(buffer,"") ;
			handle = DialogNewSimple (130, 45); 		
  		DialogAddTitle (handle, "taille x niveau 1", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "",0,2,2);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL) ;
			HeapFree (handle);

			tmp = atoi(buffer) ;
			tx = tmp ;
			if(tmp<10) fputs("0",f) ;
			fputs(buffer,f) ; // taille x

			strcpy(buffer,"") ;
			handle = DialogNewSimple (130, 45); 		
  		DialogAddTitle (handle, "taille y niveau 1", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "",0,2,2);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL) ;
			HeapFree (handle);

			tmp = atoi(buffer) ;
			ty = tmp ;
			if(tmp<10) fputs("0",f) ;
			fputs(buffer,f) ; // taille y

			clrscr() ;
			for(cpt=1; cpt<temp ; cpt++)
			{
				fputs("0000",f) ;
			} ;

			for(cpt=1;cpt<=tx*ty;cpt++)
			{
				fputs("0",f) ; // on met tout à 0
			} ;

			//fputc('\0',f) ;
			//fputc('A',f) ;
			//fputc('3',f) ;
			//fputc('3',f) ;
			//fputc('\0',f) ;
			
			
			
			
			fclose(f) ;
			DlgMessage ("Picross editor 1.0","fichier crée", BT_OK, BT_NONE);
}


	if(result == 2)
	{
			// chargement d'un niveau
  		
 		  char buffer[21] = "" ;
 		  short pos_x = 0,pos_y = 0 ;
 			short exit=0 ;
			BOOL new = 0 ;

 		  /*handle = DialogNewSimple (130, 35); 		
  		DialogAddTitle (handle, "Picross editor 1.0", BT_OK,BT_NONE);
  		DialogAddRequest (handle, 3, 15, "Enter file name",0,8,8);
	 		DialogDo (handle, CENTER, CENTER, buffer, NULL)	;	
			HeapFree (handle);*/

			strcpy(buffer,"") ;

			f = 1 ;
				FontSetSys(F_4x6) ;

			clrscr() ;

			do{			
					if(f==NULL) printf("    fichier introuvable\n\n'exit' pour quitter\n") ;
					printf("\nentrer le nom du fichier \n\n: ") ;
					
					InputStr (buffer,8) ;
		
					f = fopen(buffer,"r") ;
					if(!strcmp(buffer,"exit")) return ;
			
			} while(f==NULL)			 ;

			f = fopen(buffer,"r+") ;

			//if(!ferror(f)) { DlgMessage ("Picross editor 1.0","fichier introuvable", BT_OK, BT_NONE); return ; }
			

			fgets(buffer,14,f) ; // signature du fichier
			
			if(!strcmp(buffer,"_picross_lvl_")) //DlgMessage ("Picross editor 1.0","fichier invalide", BT_OK, BT_NONE);
			{
				// on charge le reste du niveau

				fgets(buffer,21,f) ;
				strcpy(fic.nom,buffer) ;

				strcpy(buffer,"") ;
				fgets(buffer,16,f) ;
				strcpy(fic.auteur,buffer) ;			

				strcpy(buffer,"") ;
				fgets(buffer,3,f) ;
				fic.nb_levels = atoi(buffer) ;

				fic.tab_taille_x_y = calloc(2 * 2,fic.nb_levels) ; // taille short * 2 short (tx et ty) * nb levels
				clrscr() ;
				
				pos_xy = ftell(f) ; // on sauve la pos au debut de la taille_x_y
				
				for(cpt=0;cpt<fic.nb_levels;cpt++)
				{
						strcpy(buffer,"") ;
						fgets(buffer,3,f) ;
						fic.tab_taille_x_y[cpt][0] = atoi(buffer) ;
					
						strcpy(buffer,"") ;
						fgets(buffer,3,f) ;
						fic.tab_taille_x_y[cpt][1] = atoi(buffer) ;
				} ;
				
				pos_fic = ftell(f) ; //debut des niveaux
				

				clrscr() ;
				printf("nom : %s\n",fic.nom) ;
				printf("auteur : %s\n\n",fic.auteur) ;
				printf("%i niveaux : ",fic.nb_levels) ;
				
				level = 0 ;

				printf("   %i - %ix%i",level+1,fic.tab_taille_x_y[level][0],fic.tab_taille_x_y[level][1]) ;
				printf_xy(30,70,"selection du niveau à éditer") ;
				printf_xy(45,85,"utiliser < > et enter") ;
				cpt=0 ;				
				SetIntVec (AUTO_INT_1, DUMMY_HANDLER);

				while(PRESS_ENTER) ;
				
				while(!PRESS_ENTER)
				{
						if(cpt==0)
						{	
							unsigned short old = level ;
							if((PRESS_RIGHT)&&(level!=fic.nb_levels-1)&&(fic.tab_taille_x_y[level][0] * fic.tab_taille_x_y[level][1] != 0)) level++ ;
							if((PRESS_LEFT)&&(level)) level-- ;
							
							if(old!=level)
							{
							 	printf_xy(39,18,"   %i - %ix%i  ",level+1,fic.tab_taille_x_y[level][0],fic.tab_taille_x_y[level][1]) ;
								if(fic.tab_taille_x_y[level][0] * fic.tab_taille_x_y[level][1] == 0) printf_xy(0,30,"level vide") ;
								else printf_xy(0,30,"                  ") ;
							}	
						}
						cpt++ ;
						if(cpt>=500) cpt=0 ;
						for(temp=0;temp<=1000;temp++) ;
			} ;
			
			clrscr() ;

			// si le niveau est vide
			
			if(fic.tab_taille_x_y[level][0] * fic.tab_taille_x_y[level][1] == 0)
			{
					while(PRESS_ENTER) ;
					SetIntVec (AUTO_INT_1, save_int_1);
					
					fseek (f,pos_xy,SEEK_SET);
					for(cpt=0;cpt<level;cpt++) fseek(f,4,SEEK_CUR) ; // on se déplace jusqu'à la taille x et y du niveau vide
					clrscr() ;
					
					strcpy(buffer,"") ;
					printf("\nentrer la taille x du niveau\n\n: ") ;
					
					InputStr (buffer,2) ;
		
					tmp = atoi(buffer) ;
					if(tmp<10) fputs("0",f) ;
					fputs(buffer,f) ; // taille x
					fic.tab_taille_x_y[cpt][0] = tmp ;
		
		
					strcpy(buffer,"") ;
					printf("\nentrer la taille y du niveau\n\n: ") ;
					
					InputStr (buffer,2) ;
		
					tmp = atoi(buffer) ;
					if(tmp<10) fputs("0",f) ;
					fputs(buffer,f) ; // taille y
					fic.tab_taille_x_y[cpt][1] = tmp ;
					
					fseek (f,pos_fic,SEEK_SET);
					
					SetIntVec (AUTO_INT_1, DUMMY_HANDLER);
					new = 1 ;
			}

			printf_xy(10,1," level %i : %ix%i  -  fleches, second et esc",level+1,fic.tab_taille_x_y[level][0],fic.tab_taille_x_y[level][1]) ;
			
			cpt=0 ;
										
			while(cpt!=level)
			{
				//printf("\n  %i ",cpt) ;
				temp = (fic.tab_taille_x_y[cpt][0] * fic.tab_taille_x_y[cpt][1]) ;
				fseek (f,temp,SEEK_CUR);		
				cpt ++ ;
			} ;
		
			
			fic.level = alloca(fic.tab_taille_x_y[level][0] * fic.tab_taille_x_y[level][1]) ; // y * x

			tmp=0 ;
			
			pos_fic = ftell(f) ; // on sauve la pos au début du niveau
			
		// chargement et affichage niveau
			GrayOn() ;
		for(cpt=0;cpt<fic.tab_taille_x_y[level][1];cpt++)
		{
			for(tmp=0;tmp<fic.tab_taille_x_y[level][0];tmp++)
			{
				unsigned char buf = 0 ;
				
				if(new) { buf='0' ; fputc(buf,f) ; }
				else buf = fgetc(f) ;
				
				if(buf=='1') niv.mat[cpt][tmp] = 0 ;
				if(buf=='0') niv.mat[cpt][tmp] = 1 ;
				
				afficher_sprite(niv.mat[cpt][tmp],tmp*6,cpt*6+8) ;
			};
		};	

			afficher_sprite(2,pos_x*6,pos_y*6+8) ; // affichage curseur
			
			cpt=0 ;
			
			while(!exit)
			{				
				if(cpt==0)
				{	
					if(PRESS_RIGHT)
					{
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
						pos_x++ ;
						if((unsigned)pos_x > fic.tab_taille_x_y[level][0] - 1) pos_x = fic.tab_taille_x_y[level][0] - 1 ;
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
					}
					
					if(PRESS_LEFT)
					{
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
						pos_x-- ;
						if(pos_x < 0) pos_x = 0 ;
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
					}
					
					if(PRESS_DOWN)
					{
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
						pos_y++ ;
						if((unsigned)pos_y > fic.tab_taille_x_y[level][1] - 1) pos_y = fic.tab_taille_x_y[level][1] - 1 ;
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
					}
					
					if(PRESS_UP)
					{
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
						pos_y-- ;
						if(pos_y < 0) pos_y = 0 ; ;
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
					}
				
					if(PRESS_SECOND)
					{
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
						char old = niv.mat[pos_y][pos_x] ;
						niv.mat[pos_y][pos_x] = !old ;
						afficher_sprite(!old,pos_x*6,pos_y*6+8) ;
						afficher_sprite(2,pos_x*6,pos_y*6+8) ;
						//while(PRESS_SECOND) ;
					}
					
					if(PRESS_ESCAPE) exit=1 ;
				}

				cpt++ ;
				if(cpt==1000) cpt=0 ;
				for(temp=0;temp<1500;temp++)  ;
				
			} ;

			// sauvegarde

		fseek (f,pos_fic,SEEK_SET); // on se positionne dans le fichier, au début du bon niveau

		// on sauve le niveau
		for(cpt=0;cpt<fic.tab_taille_x_y[level][1];cpt++)
		{
			for(tmp=0;tmp<fic.tab_taille_x_y[level][0];tmp++)
			{
				if(niv.mat[cpt][tmp] == 0) fputc('1',f) ;
				else fputc('0',f) ;
			};
		};	

			
			free(fic.tab_taille_x_y) ;
			fclose(f) ;

			GrayOff() ;
			while(PRESS_ESCAPE) ;
			SetIntVec (AUTO_INT_1, save_int_1);
			DlgMessage ("Picross editor 1.0","niveau sauve", BT_OK, BT_NONE);
			
		}
	}

	//SetIntVec (AUTO_INT_1, save_int_1);
}