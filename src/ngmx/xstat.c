/*
 *       $Id$
 *
 *       This source code is part of
 *
 *        G   R   O   M   A   C   S
 *
 * GROningen MAchine for Chemical Simulations
 *
 *            VERSION 1.6
 * 
 * Copyright (c) 1991-1997
 * BIOSON Research Institute, Dept. of Biophysical Chemistry
 * University of Groningen, The Netherlands
 * 
 * Please refer to:
 * GROMACS: A message-passing parallel molecular dynamics implementation
 * H.J.C. Berendsen, D. van der Spoel and R. van Drunen
 * Comp. Phys. Comm. 91, 43-56 (1995)
 *
 * Also check out our WWW page:
 * http://rugmd0.chem.rug.nl/~gmx
 * or e-mail to:
 * gromacs@chem.rug.nl
 *
 * And Hey:
 * Good gRace! Old Maple Actually Chews Slate
 */
static char *SRCID_xstat_c = "$Id$";

#include "sysstuff.h"
#include "smalloc.h"
#include "x11.h"
#include "string2.h"
#include "macros.h"
#include "fgrid.h"
#include "futil.h"
#include "xdlg.h"
#include "xdlghi.h"

typedef struct {
  int     nopt,nAppl;
  char  **name;
  char  **description;
  char  **dlgfile;
  t_dlg  *dlg;
  t_dlg  *appl;
} t_data;

static void ApplCallback(t_x11 *x11,int dlg_mess,int item_id,
			 char *set,void *dta)
{
  t_data    *data;
  t_dlg     *dlg;
  t_dlgitem *item;
  int       i;
  char      doit[1024];

  data=(t_data *)dta;
  dlg=data->appl;

  fprintf(stderr,"item_id: %d (%s)\n",item_id,set);
  if (strcasecmp(set,"OK") == 0) {
    /* Doit */
    sprintf(doit,
	    "xterm -geometry +100+100 -n %s"
	    " -title \"GROMACS: %s\" -e nice %s ",
	    data->name[data->nAppl],
	    data->name[data->nAppl],
	    data->name[data->nAppl]);
    for(i=0; (i<dlg->nitem); i++) {
      item=dlg->dlgitem[i];
      switch (item->type) {
      case edlgRB:
	strcat(doit,item->set);
	strcat(doit," ");
	break;
      case edlgCB:
	if (item->u.checkbox.bChecked)
	  strcat(doit,item->set);
	strcat(doit," ");
	break;
      case edlgET:
	if (strlen(item->u.edittext.buf) > 0) {
	  strcat(doit,item->set);
	  strcat(doit," ");
	  strcat(doit,item->u.edittext.buf);
	  strcat(doit," ");
	}
	break;
      default:
	fprintf(stderr,"Type: %d\n",item->type);
      }
    }
    strcat(doit," &");
    fprintf(stderr,"Going to exec: '%s'\n",doit);
    system(doit);
    HideDlg(data->appl);
  }
  else if (strcasecmp(set,"Cancel") == 0) {
    data->nAppl = -1;
    HideDlg(data->appl);
  }
} 

static void Callback(t_x11 *x11,int dlg_mess,int item_id,
		     char *set,void *dta)
{
  t_data *data=(t_data *)dta;

  if (item_id == data->nopt)  {
    fprintf(stderr,"Doei...\n");
    exit(0);
  }
  else {
    fprintf(stderr,"%d: %s\n",item_id,data->description[item_id]);
    if (data->nAppl != -1)
      HideDlg(data->appl);
    data->nAppl=item_id;
    data->appl=ReadDlg(x11,0,data->name[item_id],
		       BLACK,LIGHTGREY,data->dlgfile[item_id],
		       50,50,FALSE,FALSE,ApplCallback,data);
    ShowDlg(data->appl);
  }
}

static void read_opts(t_data *data)
{
  FILE *in;
  char *lib,fn[STRLEN],buf[STRLEN];
  int  i,n;

  if ((lib=getenv("GMXLIB"))==NULL) {
    fprintf(stderr,"No variable GMXLIB !\n");
    exit(1);
  }
  sprintf(fn,"%s/xstat.dat",lib);
  in=ffopen(fn,"r");
  fscanf(in,"%d",&n);
  data->nopt=n;
  snew(data->name,data->nopt);
  snew(data->description,data->nopt);
  snew(data->dlgfile,data->nopt);

  for(i=0; (i<data->nopt); i++) {
    ReadQuoteString(fn,in,buf);
    data->name[i] = strdup(buf);
    ReadQuoteString(fn,in,buf);
    data->description[i] = strdup(buf);
    ReadQuoteString(fn,in,buf);
    data->dlgfile[i] = strdup(buf);
  }
  fclose(in);
}

static void add_opts(t_x11 *x11,t_data *data)
{
  t_dlgitem *but;
  int        i,y0,w;

  y0=OFFS_Y;
  for(i=0; (i<data->nopt); i++) {
    but=CreateButton(x11,data->description[i],FALSE,
		     (t_id)i,(t_id)0,
		     OFFS_X,y0,0,0,1);
    AddDlgItem(data->dlg,but);
    y0+=but->win.height+OFFS_Y;
  }
  but=CreateButton(x11,"Quit",TRUE,(t_id)data->nopt,(t_id)0,
		   OFFS_X,y0,0,0,1);
  AddDlgItem(data->dlg,but);
  y0+=but->win.height+OFFS_Y;
  
  w=0;
  for(i=0; (i<=data->nopt); i++)
    w=max(w,QueryDlgItemW(data->dlg,i));
  w+=2*OFFS_X;
  for(i=0; (i<=data->nopt); i++)
    SetDlgItemSize(data->dlg,i,w,0);
  SetDlgSize(data->dlg,w+2*OFFS_X,y0,TRUE);
}

void main(int argc,char *argv[])
{
  t_x11  *x11;
  t_data data;

  /* Initiate X and data */
  if ((x11=GetX11(&argc,argv))==NULL) {
    fprintf(stderr,"Can't open DISPLAY\n");
    exit(1);
  }
  read_opts(&data);
  data.dlg=CreateDlg(x11,0,argv[0],0,0,0,0,0,BLACK,LIGHTGREY,Callback,&data);
  add_opts(x11,&data);
  data.nAppl=-1;

  ShowDlg(data.dlg);
  x11->MainLoop(x11);
  HideDlg(data.dlg);
}
