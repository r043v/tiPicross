// C Source File
// Created 06/07/2002; 00:46:36

#define USE_TI89              // Compile for TI-89
//#define USE_TI92PLUS          // Compile for TI-92 Plus
//#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents
	
#include <tigcclib.h>         // Include All Header Files
#include "picros.h"
//#include "key.h"

void play(short choix);
// Main Function

	unsigned short tmp ;
	unsigned long cpt ;
	struct niveau niv ;
	unsigned short taille_x = 10,taille_y = 10 ;
	FILE * f;
	long pos_fic ;
	INT_HANDLER save_int_1;

void _main(void)
{
	short choix = 0 ;
	save_int_1 = GetIntVec (AUTO_INT_1);
  SetIntVec (AUTO_INT_1, DUMMY_HANDLER);


// affichage titre
	GrayOn() ;
	FontSetSys (0);	

	afficher_titre() ;

		do
		{
			choix = afficher_menu(10,&taille_x,&taille_y) ; // affiche menu principal

		  if (choix==0 || choix==1) 
		  {
 			 	play(choix);
 			 	
 			 	FontSetSys(0);
		   	afficher_titre();
		  }

		  if (choix==2) // about
			{
				//GrayOff() ;
				//clrscr() ;
				SetPlane(DARK_PLANE) ;
				clrscr() ;
				FontSetSys (2);
				printf("\n Ti Picross") ;
				FontSetSys (1);
				printf("  beta 5 r1") ;
				printf("\n\n ecrit par rov\n\n remerciement :\n   cedric finance\n\n       www.rov.fr.st\n\n    - C 2002 RovCorp -") ;		
				while(!PRESS_ESCAPE) ;
		
				FontSetSys(0);
				afficher_titre() ;		
	}
} while(choix!=3) ;
	
	
	GrayOff() ;	
	SetIntVec (AUTO_INT_1, save_int_1);
}

void play(short choix)
{
	short temps = 1800 ;
	short pos_x = 0,pos_y = 0 ;
	short nb_case_restantes = 0 ;
	short temps_en_moins=0 ;


	if(choix==0) // si jeux aléatoire
	{
		randomize() ;
			
		for(cpt=0;cpt<taille_x;cpt++)
		{
			for(tmp=0;tmp<taille_y;tmp++)
			{
				niv.mat[tmp][cpt].couleur = random(2) ;
				niv.mat[tmp][cpt].etat = 0 ; // non dévoilé
				if(niv.mat[tmp][cpt].couleur) nb_case_restantes ++ ;		
			};
		};	
	}

	if(choix==1) // si charger niveau choisi
	{
				// chargement d'un niveau
 		  
 		  //HANDLE handle ;
 		  char buffer[21] = "" ;
 		  struct fichier fic ;
			unsigned char tmp ;
			unsigned short level ;
			unsigned short temp = 0 ;

 		  //GrayOff() ;
 		  //SetIntVec (AUTO_INT_1, save_int_1);
			clrscr() ;

			strcpy(buffer,"") ;

			//clrscr() ;

			f = 1 ;

			do{
					if(f==NULL) printf("    fichier introuvable\n\n'exit' pour quitter\n") ;
					printf("\nentrer le nom du fichier \n\n: ") ;
					
		 		  GraySetInt1Handler(save_int_1);
					InputStr (buffer,8) ;
					GraySetInt1Handler(DUMMY_HANDLER);
					
					if(!strcmp(buffer,"exit"))	return ;
		
					f = fopen(buffer,"r") ;
			
			} while(f==NULL)			 ;
			
			//SetIntVec(AUTO_INT_1, DUMMY_HANDLER);

 			fgets(buffer,14,f) ; // signature du fichier
			
			if(!strcmp(buffer,"_picross_lvl_")) //DlgMessage ("erreur","fichier invalide", BT_OK, BT_NONE);			
			{
				// on charge le reste du niveau

				fgets(buffer,21,f) ;
				strcpy(fic.nom,buffer) ;
//				strcpy(buffer,"") ;
				
				fgets(buffer,16,f) ;
				strcpy(fic.auteur,buffer) ;
				
//				strcpy(buffer,"") ;

				fgets(buffer,3,f) ;
				fic.nb_levels = atoi(buffer) ;

				fic.tab_taille_x_y = calloc(2 * 2,fic.nb_levels) ; // taille short * 2 short (tx et ty) * nb levels
				clrscr() ;
				
				
				for(cpt=0;cpt<fic.nb_levels;cpt++)
				{
//						strcpy(buffer,"") ;
						fgets(buffer,3,f) ;
						fic.tab_taille_x_y[cpt][0] = atoi(buffer) ;
					
//						strcpy(buffer,"") ;
						fgets(buffer,3,f) ;
						fic.tab_taille_x_y[cpt][1] = atoi(buffer) ;
				} ;
				
			
				FontSetSys(F_4x6) ;
				clrscr() ;
				printf("nom : %s\n",fic.nom) ;
				printf("auteur : %s\n\n",fic.auteur) ;
				printf("%i niveaux : ",fic.nb_levels) ;
				
				level = 0 ;

				printf("   %i - %ix%i",level+1,fic.tab_taille_x_y[level][0],fic.tab_taille_x_y[level][1]) ;
				printf_xy(30,70,"selection du niveau à charger") ;
				printf_xy(45,85,"utiliser < > puis enter") ;
				cpt=0 ;

				while(PRESS_ENTER)	;
		//	SetIntVec (AUTO_INT_1, DUMMY_HANDLER);
				while(!PRESS_ENTER)
				{
						if(cpt==0)
						{	
							unsigned short old = level ;
							if((PRESS_RIGHT)&&(level!=fic.nb_levels-1)&&(fic.tab_taille_x_y[level + 1][0] * fic.tab_taille_x_y[level + 1][1] != 0)) level++ ;
							if((PRESS_LEFT)&&(level)) level-- ;
							
							if(old!=level)
							{
							 	printf_xy(39,18,"   %i - %ix%i    ",level+1,fic.tab_taille_x_y[level][0],fic.tab_taille_x_y[level][1]) ;
							}	
						}
						cpt++ ;
						if(cpt>=500) cpt=0 ;
						for(temp=0;temp<=1000;temp++) ;
				} ;
		//		SetIntVec (AUTO_INT_1, save_int_1);
		
		cpt=0 ; // level actuel = 0
			
			pos_fic = ftell(f) ;
										
			while(cpt!=level)
			{
				// on avance dans le fichier jusq'au niveau voulu
				temp = (fic.tab_taille_x_y[cpt][0] * fic.tab_taille_x_y[cpt][1]) ;
				fseek (f,temp,SEEK_CUR);		
				cpt ++ ;
			} ;
			
		taille_x = fic.tab_taille_x_y[level][0] ; // taille_x = celle chargée
		taille_y = fic.tab_taille_x_y[level][1] ; // taille_y = celle chargée
		clrscr() ;	
		
		for(cpt=0;cpt<taille_y;cpt++)
		{
			for(tmp=0;tmp<taille_x;tmp++)
			{
				unsigned char buf = 0 ;
				buf = fgetc(f) ;
				
				if(buf=='1') niv.mat[cpt][tmp].couleur = 1 ;
				if(buf=='0') niv.mat[cpt][tmp].couleur = 0 ;
				
				//printf(" %i ",niv.mat[cpt][tmp].couleur) ;
				
				niv.mat[cpt][tmp].etat = 0 ; // non dévoilé
				if(niv.mat[cpt][tmp].couleur) nb_case_restantes ++ ;		
			};
			//printf("\n") ;
		};	
				
				free(fic.tab_taille_x_y) ;
				
		}
		
  		//	SetIntVec (AUTO_INT_1, DUMMY_HANDLER);
				fclose(f) ;
//				GrayOn() ;
	}


	niv.taille_x = taille_x ;
	niv.taille_y = taille_y ;
	
	cpt = 0 ;

	calculer_niveau(&niv);
	
	afficher_niveau(&niv) ;
				
	OSFreeTimer (USER_TIMER);
  OSRegisterTimer (USER_TIMER,27);			// 1 sec
	
	afficher_temps(temps) ;
	
	aff_sprt ;			
	
	do
	{
		cpt=0 ;
		
		tmp = 1 ;
		while((tmp)&&(temps>0))
		{

				cpt++ ;
				if(PRESS_DIAM && (cpt >= 2500)) cpt=1 ;
				if(cpt==6000) cpt=1 ;
				if(cpt==1) tmp=lire_clavier(&pos_x,&pos_y,&niv,&temps,&nb_case_restantes,&temps_en_moins) ; // lecture des touches
						
		  if(OSTimerExpired (USER_TIMER)) // actualise le compte à rebourg
		  {
				temps -- ;
				if(temps<0) temps = 0 ;
		  	afficher_temps(temps) ;

				if(!tmp)
				{
					SetPlane (0);
			  	printf_xy(30,LCD_HEIGHT - 6,"                   ") ;
					SetPlane (1);					  	
		  		printf_xy(30,LCD_HEIGHT - 6,"                   ") ;
				}
			}
			
			
		}
		//printf_xy(120,80,"case %i   ",nb_case_restantes) ;
		if(PRESS_ESCAPE)
		{
			temps = 0 ;
			afficher_temps(temps) ;
			while(PRESS_ESCAPE) ;
		}

	} while((nb_case_restantes)&&(temps>0)) ; 
 
  OSFreeTimer (USER_TIMER);

	if(!nb_case_restantes) // si gagné
	{
		Image_C0 = &win_C0;
		Image_C1 = &win_C1;
		
		SetPlane(LIGHT_PLANE) ;
		BitmapPut ((LCD_WIDTH - 64)/2,(LCD_HEIGHT - 21)/2,Image_C0,ScrRect,A_REPLACE);
		SetPlane(DARK_PLANE) ;
		BitmapPut ((LCD_WIDTH - 64)/2,(LCD_HEIGHT - 21)/2,Image_C1,ScrRect,A_REPLACE);
	}

	if(!temps) // si perdu
	{
		Image_C0 = &perdu_C0;
		Image_C1 = &perdu_C1;

		SetPlane(LIGHT_PLANE) ;
		BitmapPut ((LCD_WIDTH - 80)/2,(LCD_HEIGHT - 19)/2,Image_C0,ScrRect,A_REPLACE);
		SetPlane(DARK_PLANE) ;
		BitmapPut ((LCD_WIDTH - 80)/2,(LCD_HEIGHT - 19)/2,Image_C1,ScrRect,A_REPLACE);	
	}	
	while(!PRESS_ESCAPE);
	while(PRESS_ESCAPE);
}