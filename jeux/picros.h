// C Header File
// Created 06/07/2002; 02:01:56

#include "util.h"

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

void afficher_titre(void)
{
	SetPlane(0); 
	ClrScr(); 
	BitmapPut ((LCD_WIDTH - 140)/2,(LCD_HEIGHT - 95)/2, &titre_C0, ScrRect, A_NORMAL); 
	SetPlane(1); 
	ClrScr(); 
	BitmapPut ((LCD_WIDTH - 140)/2,(LCD_HEIGHT - 95)/2, &titre_C1, ScrRect, A_NORMAL); 
}

void afficher_sprite_ch(BOOL couleur,short x,short y)
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

	if(couleur)  // case dévoilée blanche
	{
		Image_C0 = &case_b_C0;
		Image_C1 = &case_n_d_C1;
		SetPlane(LIGHT_PLANE) ;
		BitmapPut (x,y,Image_C0,ScrRect,A_REPLACE);
		SetPlane(DARK_PLANE) ;
		BitmapPut (x,y,Image_C1,ScrRect,A_REPLACE);
	}
}

short afficher_menu(short tmp,short *tx,short *ty)
{
			short choix = 0,cpt ;
			if(tmp==0) 	printf_xy(LCD_WIDTH/2-30,LCD_HEIGHT - 6," _ jeux aléatoire _   ") ;
			
			if(tmp==1)	printf_xy(LCD_WIDTH/2-30,LCD_HEIGHT - 6," _ charger niveau _   ") ;
			
			if(tmp==2)	printf_xy(LCD_WIDTH/2-30,LCD_HEIGHT - 6,"   __ à propos __       ") ;
			
			if(tmp==3)	printf_xy(LCD_WIDTH/2-30,LCD_HEIGHT - 6,"   __ quitter __       ") ;

			if(tmp==4)
			{
				//printf_xy(45,95,"taille jeux : %i x %i    ",tx,ty) ;
				
				if((*tx<10)&&(*ty<10))		printf_xy(LCD_WIDTH/2-35,LCD_HEIGHT - 6,"taille jeux : 0%i x 0%i    ",*tx,*ty) ;
				if((*tx<10)&&(*ty>=10))		printf_xy(LCD_WIDTH/2-35,LCD_HEIGHT - 6,"taille jeux : 0%i x %i    ",*tx,*ty) ;
				if((*tx>=10)&&(*ty<10))		printf_xy(LCD_WIDTH/2-35,LCD_HEIGHT - 6,"taille jeux : %i x 0%i    ",*tx,*ty) ;
				if((*tx>=10)&&(*ty>=10))	printf_xy(LCD_WIDTH/2-35,LCD_HEIGHT - 6,"taille jeux : %i x %i    ",*tx,*ty) ;
			}
			
			if(tmp==10) // menu principal
			{
				// affichage menu principal

			 	afficher_menu(choix,0,0) ;
	
			  while(!PRESS_ENTER)
	  		{
	  			cpt = 0 ;
	
					if (PRESS_ESCAPE)
					{ 
						choix=3;
					  return choix;
					}
					while(PRESS_RIGHT)
					{
						cpt++ ;
						if(cpt==17000) cpt=1 ;
						if(cpt==1) { choix++ ; if(choix>=4) choix=0 ; afficher_menu(choix,0,0) ; }
					}		
	
					while(PRESS_LEFT)
					{
						cpt++ ;
						if(cpt==17000) cpt=1 ;
						if(cpt==1) { choix-- ; if(choix<=-1) choix=3 ; afficher_menu(choix,0,0) ; }
					}		
				}
	
			  while(PRESS_ENTER) ;

				if(choix==0)
				{
					afficher_menu(11,tx,ty) ; // affiche menu jeux aléatoire
				}
				
				return choix ;
				
			}
			
			if(tmp==11)
			{
				// affichage menu jeux aléatoire

				afficher_menu(4,tx,ty) ;
			  
			  while(!PRESS_ENTER)
			  {
			  	cpt = 0 ;
					while(PRESS_RIGHT)
					{
						cpt++ ;
						if(cpt==7000) cpt=1 ;
						if(cpt==1) { (*tx)++ ; if(*tx>PZMAX_X) *tx=PZMAX_X ; afficher_menu(4,tx,ty) ; }
					}		
			
					while(PRESS_LEFT)
					{
						cpt++ ;
						if(cpt==7000) cpt=1 ;
						if(cpt==1) { (*tx)-- ; if(*tx<=0) *tx=1 ; afficher_menu(4,tx,ty) ; }
					}		
			
					while(PRESS_UP)
					{
						cpt++ ;
						if(cpt==7000) cpt=1 ;
						if(cpt==1) { (*ty)++ ; if(*ty>PZMAX_Y) *ty=PZMAX_Y ; afficher_menu(4,tx,ty) ; }
					}		
			
					while(PRESS_DOWN)
					{
						cpt++ ;
						if(cpt==7000) cpt=1 ;
						if(cpt==1) { (*ty)-- ; if(*ty<=0) *ty=1 ; afficher_menu(4,tx,ty) ; }
					}		
				}	
			}

	return 9 ;
}

void afficher_temps(short temps)
{
  	short sec, min ;

  	min = temps / 60 ;
  	sec = temps - 60 * min ;  	

		if((min<10)&&(sec<10))		printf_xy (0,LCD_HEIGHT - 6,"0%i : 0%i",min,sec) ;
		if((min<10)&&(sec>=10))		printf_xy (0,LCD_HEIGHT - 6,"0%i : %i",min,sec) ;
		if((min>=10)&&(sec<10))		printf_xy (0,LCD_HEIGHT - 6,"%i : 0%i",min,sec) ;
		if((min>=10)&&(sec>=10))	printf_xy (0,LCD_HEIGHT - 6,"%i : %i",min,sec) ;
}

void afficher_sprite(short couleur,short x,short y)
{
	if(couleur==0)  // case dévoilée noire
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

	if(couleur==2)  // case marquée
	{
		Image_C0 = &case_m_C0;
		Image_C1 = &case_m_C1;
		
		SetPlane(LIGHT_PLANE) ;
		BitmapPut (x,y,Image_C0,ScrRect, A_REPLACE);
		SetPlane(DARK_PLANE) ;
		BitmapPut (x,y,Image_C1,ScrRect, A_REPLACE);
	}

	if(couleur==3)  // case non dévoilée
	{
		Image_C0 = &case_n_d_C0;
		Image_C1 = &case_n_d_C1;
		
		SetPlane(LIGHT_PLANE) ;
		BitmapPut (x,y,Image_C0,ScrRect, A_REPLACE); 
		SetPlane(DARK_PLANE) ;
		BitmapPut (x,y,Image_C1,ScrRect, A_REPLACE); 
	}
	
	if(couleur==4)  // curseur
	{
		static unsigned char sprite_light[] ={ 0x2, 0x1, 0x1, 0x21, 0x2E, 0x18, 0x20, 0xC0};
		static unsigned char sprite_dark[] ={ 0xE, 0x11, 0x11, 0x31, 0x3E, 0x78, 0xE0, 0xC0};

		Sprite8 (x + 2,y - 5,8,sprite_light, GetPlane(LIGHT_PLANE), SPRT_XOR);
		Sprite8 (x + 2,y - 5,8,sprite_dark, GetPlane(DARK_PLANE), SPRT_XOR);
	}
}

/*///////////////////////////////////////////////////////////////////*/

void afficher_case(struct niveau *niv,short pos_x,short pos_y)
{
   					  short etat = niv->mat[pos_y][pos_x].etat ;
    				  short x = niv->img_mario_x  ;
    				  short y = niv->img_mario_y  ;
							
							afficher_sprite(4,taille_case*pos_x + niv->img_mario_x + 1,taille_case*(niv->aff_y) + y + 1) ;
   						if(etat == 0) afficher_sprite(3,taille_case*pos_x + x,taille_case*(niv->aff_y) + y) ; // case non dévoilée
							if(etat == 1) afficher_sprite(!(BOOL)niv->mat[pos_y][pos_x].couleur,taille_case*pos_x + x,taille_case*(niv->aff_y) + y) ; // case dévoilée
							if(etat == 2) afficher_sprite(2,taille_case*pos_x + x,taille_case*(niv->aff_y) + y) ; // case marquée
							afficher_sprite(4,taille_case*pos_x + niv->img_mario_x + 1,taille_case*(niv->aff_y) + y + 1) ;
}

/*///////////////////////////////////////////////////////////////////*/

void calculer_ligne(struct case_tab *matrice,short *result,short *nombre)
{
        short pos = 0 ;
        short nb = 0 ;
        short rang = 0 ;
        short taille ;
        short taille_result ;
        short buffer ;

        taille = *nombre ;
        taille_result = taille / sizeof(taille) + 1;
				*nombre = 0 ;

        for(pos=0;pos<taille;pos++)
        {
                buffer = matrice[pos].couleur ;
                if(buffer) nb++ ;
                if((!buffer)&&(nb))
                {
                        result[rang] = nb ;
                        (*nombre) ++ ;
                        rang++ ;
                        nb = 0 ;
                }
        } ;

        if(nb)
        {
                result[rang] = nb ;
                (*nombre) ++ ;
        }
}

/*///////////////////////////////////////////////////////////////////*/

void afficher_niveau(struct niveau *niv) // affiche le jeux et les info des lignes
{
	short cpt_x,cpt_y,tmp=1,nb,cpt_ligne,etat,couleur ;
	short buffer ;
	//niv->scrolling = 0 ;

	for(cpt_y=0;(cpt_y < niv->taille_y)&&(cpt_y < MAX_Y - 1);cpt_y++)
	{
		//cpt_ligne = niv->aff_y + niv->scrolling ;
		cpt_ligne=cpt_y + niv->scrolling ;
		// on affiche la ligne de bmp
		for(cpt_x=0;cpt_x<niv->taille_x;cpt_x++)
		{
			etat = niv->mat[cpt_ligne][cpt_x].etat ;
			couleur = niv->mat[cpt_ligne][cpt_x].couleur ;
			
			if(etat == 0) afficher_sprite(3,taille_case*cpt_x + niv->img_mario_x,taille_case*cpt_y + niv->img_mario_y) ; // case non dévoilée
			if(etat == 1) afficher_sprite(!(BOOL)couleur,taille_case*cpt_x + niv->img_mario_x,taille_case*cpt_y + niv->img_mario_y) ; // case dévoilée
			if(etat == 2) afficher_sprite(2,taille_case*cpt_x + niv->img_mario_x,taille_case*cpt_y + niv->img_mario_y) ; // case marquée
		};

		if(!tmp) // on alterne les col blanche et noire
		{
			Image_C1 = &ligne_w_C1 ;
			Image_C0 = &ligne_w_C0 ;
			tmp++ ;
		}
		else
		{
			Image_C1 = &ligne_b_C1 ;
			Image_C0 = &ligne_b_C0 ;
			tmp-- ;
		}

		SetPlane (0);
		BitmapPut (0,niv->img_mario_y + cpt_y * 6,Image_C0, ScrRect, A_REPLACE);
		SetPlane (1);
		BitmapPut (0,niv->img_mario_y + cpt_y * 6,Image_C1, ScrRect, A_REPLACE);

		nb = niv->nb_ligne[cpt_ligne]	;

		for(cpt_x=0;cpt_x<nb;cpt_x++)
		{
			buffer = niv->result_ligne[cpt_ligne][cpt_x] ;
			printf_xy(cpt_x * 4 + 2,niv->img_mario_y + cpt_y * 6 + 1,"%X",buffer) ;
		};
			// si pas d'info
			if(cpt_x==0) { SetPlane (!tmp); printf_xy(2,niv->img_mario_y + cpt_y * 6 + 1,"-") ; } ;
	};	
}

/*///////////////////////////////////////////////////////////////////*/

void afficher_info_col(struct niveau * niv)
{
				short cpt_y,cpt_x,tmp=1,nb,buffer,aff_y;
				
				for(cpt_y = 0;cpt_y<niv->taille_x;cpt_y++)
				{
					if(!tmp) // on alterne les col blanche et noire
					{
						Image_C1 = &col_w_C1 ;
						Image_C0 = &col_w_C0 ;
						tmp++ ;
					}
					else
					{
						Image_C1 = &col_b_C1 ;
						Image_C0 = &col_b_C0 ;					
						tmp-- ;
					}
					
					SetPlane (0);
					BitmapPut (niv->img_mario_x + cpt_y * 6,0, Image_C0, ScrRect, A_REPLACE);
					SetPlane (1);
					BitmapPut (niv->img_mario_x + cpt_y * 6,0, Image_C1, ScrRect, A_REPLACE);
				
					nb = niv->nb_col[cpt_y]	;
					aff_y=0 ;
					for(cpt_x=niv->scroll_info_col;(cpt_x<nb)&&(cpt_x - niv->scroll_info_col < 5);cpt_x++)
					{
						buffer = niv->result_col[cpt_y][cpt_x] ;
						SetPlane (!tmp);
						printf_xy(niv->img_mario_x + cpt_y * 6,1 + aff_y * 6,"%X",buffer) ;
						if(tmp)
						{
							SetPlane (tmp);
							printf_xy(niv->img_mario_x + cpt_y * 6,1 + aff_y * 6,"%X",buffer) ;
						}
						aff_y++ ;
					} ;					
							// si pas d'info
							if(cpt_x==0) { SetPlane (!tmp); printf_xy(niv->img_mario_x + cpt_y * 6,1,"-") ; } ; 
				} ;
}

//////////////////////////////////////////////////////////////////////////////////////////

void calculer_niveau(struct niveau *niv)
{
				short *img_mario_x = &niv->img_mario_x ;
				short *img_mario_y = &niv->img_mario_y ;
				short taille_x = niv->taille_x ;
				short taille_y = niv->taille_y ;
				struct case_tab imat[taille_x][taille_y] ;
				//short result_y[taille_x][taille_y / 2 + 1] ;
				short nb_y[taille_x] ;
				short taille_result_x,taille_result_y ;
				short cpt_x,cpt_y,nb ;
				short nb_max_x = 0;
				
				niv->nb_max_y = 0 ;				
				//short buffer,tmp = 0 ;
				
				/*  inversion de la matrice  */
				
				for(cpt_x=0;cpt_x<taille_x;cpt_x++)
				{
					for(cpt_y=0;cpt_y<taille_y;cpt_y++)
					{
						imat[cpt_x][cpt_y].couleur = niv->mat[cpt_y][cpt_x].couleur ;
					};				
				};
				
				/*	matrice pour les indices	*/
				
				taille_result_x = taille_x / 2 + 1 ;
				taille_result_y = taille_y / 2 + 1 ;

				/*  calcul des collones  */
				
				for(cpt_x=0;cpt_x<taille_x;cpt_x++)
				{
					nb = taille_y ;
					calculer_ligne(imat[cpt_x],niv->result_col[cpt_x],&nb) ;
					
					if(nb>niv->nb_max_y) niv->nb_max_y = nb ;
					nb_y[cpt_x] = nb ;
					niv->nb_col[cpt_x] = nb ;
				};
				
				/*  calcul des lignes  */
				
				for(cpt_y=0;cpt_y<taille_y;cpt_y++)
				{
					nb = taille_x ;
					calculer_ligne(niv->mat[cpt_y],niv->result_ligne[cpt_y],&nb) ;
					if(nb>nb_max_x) nb_max_x = nb ;
					niv->nb_ligne[cpt_y] = nb ;
				};
				
				niv->nb_max_col = niv->nb_max_y ;
				niv->nb_max_ligne = nb_max_x ;
				
				// affichage image mario				

				*img_mario_x = 42 ;
				*img_mario_y = 32 ;

				Image_C1 = &mario_m_C1 ; // pour du 10x10 max (nb d'info), img plus grosse (ou la mm centrée) pour du 15x15
				Image_C0 = &mario_m_C0 ;

				SetPlane(1); 
				clrscr() ;
				BitmapPut (0, 0, Image_C1, ScrRect, A_NORMAL); 	

				SetPlane(0); 
				ClrScr();
				BitmapPut (0, 0, Image_C0, ScrRect, A_NORMAL); 	
				
				niv->scrolling = 0 ; // pas de scrolling au début
				niv->scroll_info_col = 0 ;
				niv->aff_y = 0 ;
				
				afficher_info_col(niv) ;
}

/*///////////////////////////////////////////////////////////////////*/



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

BOOL lire_clavier(short * pos_x,short * pos_y,struct niveau * niv, short * temps,short * nb_case_restantes,short * temps_en_moins)
{
		static BOOL marque = 0 ;
		static short nb_tmp = 0 ;
		//static short aff_y = 0;
		BOOL tmp=0 ;
		
		// etteint la calc si 'on'
		asm("btst.b #1,0x60001A
			bne lire_clavier_not_on_key
			trap #4
			move.b #0xFF,0x600003
			lire_clavier_not_on_key:");

		if(PRESS_RIGHT)
		{
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			(*pos_x)++ ;
			if(*pos_x > niv->taille_x-1) *pos_x = 0 ;
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			tmp = 1 ;
			marque = 0 ;		
		}
		
		if(PRESS_LEFT)
		{
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			(*pos_x)-- ;
			if(*pos_x < 0) *pos_x = niv->taille_x-1 ;
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			tmp = 1 ;
			marque = 0 ;
		}


		if((PRESS_UP)&&(*pos_y != 0)&&(!PRESS_F1)) // up
		{
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			
			if((niv->aff_y)==0) {  niv->scrolling-- ; afficher_niveau(niv) ; }
			else {	niv->aff_y-- ;	}

			tmp = 1 ;
			marque = 0 ;
			(*pos_y)-- ;
			niv->pos_y-- ;
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			//printf_xy(130,0,"%i %i %i  ",niv->pos_y,niv->aff_y,niv->scrolling) ;
		}


		if((PRESS_DOWN)&&(*pos_y < niv->taille_y-1)&&(!PRESS_F1)) // down
		{
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			
			if((niv->aff_y)==MAX_Y-2) {  niv->scrolling++ ; afficher_niveau(niv) ; }
			else {	niv->aff_y++ ;	}

			tmp = 1 ;
			marque = 0 ;
			(*pos_y)++ ;
			niv->pos_y++ ;
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			//printf_xy(130,0,"%i %i %i  ",niv->pos_y,niv->aff_y,niv->scrolling) ;
		}

		if((PRESS_UP)&&(PRESS_F1)&&(niv->scroll_info_col > 0)) // scroll info col up
		{
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;			
			niv->scroll_info_col-- ;
			afficher_info_col(niv) ;
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
		}

		if((PRESS_DOWN)&&(PRESS_F1)&&(niv->scroll_info_col < niv->nb_max_y - 5 )) // scroll info col down
		{
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
			niv->scroll_info_col++ ;
			afficher_info_col(niv) ;
			afficher_sprite(4,taille_case * *pos_x + niv->img_mario_x + 1,taille_case * niv->aff_y + niv->img_mario_y + 1) ;
		}

		if(PRESS_SECOND)
		{
				char old_etat = niv->mat[*pos_y][*pos_x].etat ; // état précédent de la case
				char couleur = niv->mat[*pos_y][*pos_x].couleur ; // couleur de la case
			
				if(old_etat!=1)
				{
						niv->mat[*pos_y][*pos_x].etat = 1 ; // on met l'état de la case dévoilée
				
						if(!couleur)
						{
								short p_t = 2 ;
								*temps -= *temps_en_moins * 60 ;
								if(*temps_en_moins<8) *temps_en_moins += 2 ;
						  			if(*temps<0) *temps = 0 ;
						  	afficher_temps(*temps) ;
						  	if(*temps_en_moins<8) p_t = *temps_en_moins + 2 ;
						  	else p_t = 8 ;
						  	SetPlane (0);
							  	printf_xy(30,LCD_HEIGHT - 6,"- %i minutes",*temps_en_moins) ;
								SetPlane (1);					  	
						  		printf_xy(30,LCD_HEIGHT - 6,"- %i minutes   (%i)",*temps_en_moins,p_t) ;
						  		OSFreeTimer (USER_TIMER);
						  		OSRegisterTimer (USER_TIMER,27);						  	
						}
						
						afficher_case(niv,*pos_x,*pos_y) ;
				}
						if(((old_etat == 0)||(old_etat == 2))&&(couleur == 1))
						{
							(*nb_case_restantes) -- ;
							//printf("nb_case\n") ;
						}
		}

		if((PRESS_SHIFT)&&(!marque))
	  {
				char old_etat = niv->mat[*pos_y][*pos_x].etat ; // état précédent de la case
				if(old_etat == 0) niv->mat[*pos_y][*pos_x].etat = 2 ; // si case non dévoilée
				if(old_etat == 2) niv->mat[*pos_y][*pos_x].etat = 0 ; // si case déjà marquée			
				afficher_case(niv,*pos_x,*pos_y) ;
				marque = 1 ;
				nb_tmp=0 ;
	  }
		
if(!tmp) nb_tmp++ ;
if(nb_tmp>=2000) { marque = 0 ; nb_tmp=0 ; }


	return tmp ;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++





