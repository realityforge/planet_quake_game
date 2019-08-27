// =========================
// SHUD : HTML stats logging
// =========================

#include "cg_local.h"

char *weapname[] = {"Gauntlet","Machinegun","Shotgun","Grenade launcher","Rocket launcher","Lightning gun","Railgun","Plasma gun","BFG"};
char *ab_weapname[] = {"GT","MG","SG","GL","RL","LG","RG","PG","BFG"};


// Test tag and extract parameter.
// Example : <!--SH_Cvar:timelimit-->
qboolean CG_SHUDTagWithParm(char *tag, char *tagname, char *parm_out)
{
	char tmp[256];
 	int i;
  	char *ptr;
   
	if(strlen(tag)<5) return qfalse;
 	Com_sprintf(tmp,sizeof(tmp),"!--%s",tagname);
  	i=0;
   	while(i<strlen(tmp))
    	{
     		if(tmp[i]!=tag[i]) return qfalse;
       	i++;
      };
      ptr = strchr(tag,':');
      if(!ptr) return qfalse;
      if(tag[strlen(tag)-1]!='-') return qfalse;
      if(tag[strlen(tag)-2]!='-') return qfalse;
      strcpy(parm_out,&ptr[1]);
      parm_out[strlen(parm_out)-2]=0;
      return qtrue;
}


// Self-explanatory ...
void CG_SHUDConvertFunNameToHTML(char *FunName, char *HTMLString)
{
	char *ptr;
 	char name[256];
  	int i;
  	qboolean first;
   
  	HTMLString[0]=0;
  	strcpy(name,FunName);
    	ptr = strchr(name,'^');
  	first = qtrue;
  	while(ptr)
   	{
	    ptr[0]=0; strcat(HTMLString,name);
     	    if (!first) strcat(HTMLString,"</font>");
          first = qfalse;
     	    switch(ptr[1])
          {
          	case '0':
           		strcat(HTMLString,"<font color = \"000000\">");
             	break;
          	case '1':
           		strcat(HTMLString,"<font color = \"FF0000\">");
             	break;
          	case '2':
           		strcat(HTMLString,"<font color = \"00FF00\">");
             	break;
          	case '3':
           		strcat(HTMLString,"<font color = \"FFFF00\">");
             	break;
          	case '4':
           		strcat(HTMLString,"<font color = \"0000FF\">");
             	break;
        	case '5':
           		strcat(HTMLString,"<font color = \"00FFFF\">");
             	break;
          	case '6':
           		strcat(HTMLString,"<font color = \"FF00FF\">");
             	break;
          	case '7':
           		strcat(HTMLString,"<font color = \"FFFFFF\">");
             	break;
            default :   	
            	break;         
          };
          strcpy(name,&ptr[2]);
 	    ptr = strchr(name,'^');
    	};
     	strcat(HTMLString,name);	
     	if (!first) strcat(HTMLString,"</font>");
}

// ConvertTag : used to analyse the contents of an HTML tag.
// Issues the write operations depending on the tag encountered.
// BEWARE : tags are case-sensitive !

void CG_SHUDConvertTag(char *tag, fileHandle_t out)
{
	char str_out[2048];
 	char tmp[512];
  	char HTMLName[512];
   	char parm[256];
    	char *ptr;
 	int i,j,k,p,pf,msec,csec,fph,dfph,weapcount,numcols,client;
  

  	// Normal accuracy table
	if(strcmp(tag,"!--SH_AccuracyTable1--")==0) 
 	{
		strcpy(str_out,"<table cols = 4 border>\r\n<tr><th>Weapon</th><th>Fired</th><th>Hits</th><th>Accuracy</th></tr>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
    		for(i=2;i<10;i++)if ( cg_weapons[i].weaponIcon ) 
      	{
       		if(acc_weap_f[i]==0) {p=0;pf=0;}
         		else
           		{
         			p = (acc_weap_h[i]*100)/acc_weap_f[i];
         			pf = ((acc_weap_h[i]*1000)/acc_weap_f[i])-p*10;
            	};
       		if(pf!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> </tr>\r\n",weapname[i-1],acc_weap_f[i],acc_weap_h[i],p,pf);
         		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>\r\n",weapname[i-1],acc_weap_f[i],acc_weap_h[i],p);
			trap_FS_Write(str_out,strlen(str_out),out);
       	};
       	if(cg.snap->ps.persistant[PERS_ACCURACY_SHOTS]==0) {p=0;pf=0;}
         	else
            {
         		p = (cg.snap->ps.persistant[PERS_ACCURACY_HITS]*100)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS];
         		pf = ((cg.snap->ps.persistant[PERS_ACCURACY_HITS]*1000)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS])-p*10;
            };
       	if(pf!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>All weapons</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_ACCURACY_SHOTS],cg.snap->ps.persistant[PERS_ACCURACY_HITS],p,pf);
         	else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>All weapons</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_ACCURACY_SHOTS],cg.snap->ps.persistant[PERS_ACCURACY_HITS],p);
  		trap_FS_Write(str_out,strlen(str_out),out);
		strcpy(str_out,"</table>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
   
	// Enhanced accuracy table
	else if(strcmp(tag,"!--SH_AccuracyTable2--")==0)
 	{
		strcpy(str_out,"<table cols = 5 border>\r\n<tr><th>Weapon</th><th>Fired</th><th>Hits</th><th>Max.c.hits</th><th>Accuracy</th></tr>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
    		for(i=2;i<10;i++)if ( cg_weapons[i].weaponIcon ) 
      	{
       		if(acc_weap_f[i]==0) {p=0;pf=0;}
         		else
           		{
         			p = (acc_weap_h[i]*100)/acc_weap_f[i];
         			pf = ((acc_weap_h[i]*1000)/acc_weap_f[i])-p*10;
            	};
       		if(pf!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> </tr>\r\n",weapname[i-1],acc_weap_f[i],acc_weap_h[i],max_cons_hits[i],p,pf);
         		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>\r\n",weapname[i-1],acc_weap_f[i],acc_weap_h[i],max_cons_hits[i],p);
			trap_FS_Write(str_out,strlen(str_out),out);
       	};
       	if(cg.snap->ps.persistant[PERS_ACCURACY_SHOTS]==0) {p=0;pf=0;}
         	else
            {
         		p = (cg.snap->ps.persistant[PERS_ACCURACY_HITS]*100)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS];
         		pf = ((cg.snap->ps.persistant[PERS_ACCURACY_HITS]*1000)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS])-p*10;
            };
       	if(pf!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>All weapons</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_ACCURACY_SHOTS],cg.snap->ps.persistant[PERS_ACCURACY_HITS],max_cons_hits_all,p,pf);
         	else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>All weapons</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_ACCURACY_SHOTS],cg.snap->ps.persistant[PERS_ACCURACY_HITS],max_cons_hits_all,p);
  		trap_FS_Write(str_out,strlen(str_out),out);
		strcpy(str_out,"</table>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
   

	// Rewards table
	else if(strcmp(tag,"!--SH_RewardsTable--")==0)
 	{
		strcpy(str_out,"<table cols = 2 border>\r\n<tr><th>Reward</th><th>Number</th></tr>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);

    		// Impressive
		Com_sprintf(str_out,sizeof(str_out),"<tr> <td>Impressive</td> <td>%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_IMPRESSIVE_COUNT]);
  		trap_FS_Write(str_out,strlen(str_out),out);

    		// Excellent
		Com_sprintf(str_out,sizeof(str_out),"<tr> <td>Excellent</td> <td>%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_EXCELLENT_COUNT]);
  		trap_FS_Write(str_out,strlen(str_out),out);

    		// Humiliation
		Com_sprintf(str_out,sizeof(str_out),"<tr> <td>Gauntlet frags</td> <td>%i</td> </tr>\r\n",cg.snap->ps.persistant[PERS_GAUNTLET_FRAG_COUNT]);
  		trap_FS_Write(str_out,strlen(str_out),out);

    		// Frags per hour
 		if(cg.snap->ps.persistant[PERS_SCORE]!=0)
   		{
     			client = cg.snap->ps.clientNum;
     			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>Frag rate</td> <td>%i.%i</td> </tr>\r\n",stats[client].fph,stats[client].dfph);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>Frag rate</td> <td>%i</td> </tr>\r\n",stats[client].fph);
   		} else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>Frag rate</td> <td>%i</td> </tr>\r\n",0);
     		trap_FS_Write(str_out,strlen(str_out),out);

		strcpy(str_out,"</table>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
      
	// Scores table 1 (weapon frags)
	else if(strcmp(tag,"!--SH_ScoresTable1--")==0)
 	{
  		weapcount = 0;
  		for(i=2;i<10;i++)if ( cg_weapons[i].weaponIcon ) weapcount++;
    		numcols = weapcount+9;
		Com_sprintf(str_out,sizeof(str_out),"<table cols = %i border>\r\n<tr><th>Name</th><th>Time</th><th>Score</th><th>FR</th><th>Eff.</th></th>",numcols);
  		for(i=1;i<10;i++)if ( cg_weapons[i].weaponIcon ) 
    		{
      		Com_sprintf(tmp,sizeof(tmp),"<th>%s</th>",ab_weapname[i-1]);
        		strcat(str_out,tmp);
      	};
    		strcat(str_out,"<th>Exc.</th><th>Kills</th><th>Dth.</th><th>Suic.</th></tr>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
		
		for(i=0;i<cg.numScores;i++) if(cgs.clientinfo[cg.scores[i].client].team==TEAM_BLUE) 
  		{
     			client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].dfph,stats[client].efficiency);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].efficiency);
        		trap_FS_Write(str_out,strlen(str_out),out);
      		for(j=1;j<10;j++) if (cg_weapons[j].weaponIcon) 
        		{
				Com_sprintf(tmp,sizeof(tmp)," <td>%i</td>",stats[client].weapon[j-1]);
  				trap_FS_Write(tmp,strlen(tmp),out);
   			};
      		Com_sprintf(tmp,sizeof(tmp)," <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>",stats[client].excellent,stats[client].kills,stats[client].killed,stats[client].frags[client]);
  			trap_FS_Write(tmp,strlen(tmp),out);
    		};
 		for(i=0;i<cg.numScores;i++) if(cgs.clientinfo[cg.scores[i].client].team==TEAM_RED) 
  		{
     			client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].dfph,stats[client].efficiency);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].efficiency);
        		trap_FS_Write(str_out,strlen(str_out),out);
      		for(j=1;j<10;j++) if (cg_weapons[j].weaponIcon) 
        		{
				Com_sprintf(tmp,sizeof(tmp)," <td>%i</td>",stats[client].weapon[j-1]);
  				trap_FS_Write(tmp,strlen(tmp),out);
   			};
      		Com_sprintf(tmp,sizeof(tmp)," <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>",stats[client].excellent,stats[client].kills,stats[client].killed,stats[client].frags[client]);
  			trap_FS_Write(tmp,strlen(tmp),out);
    		};
 		for(i=0;i<cg.numScores;i++) 
   		if((cgs.clientinfo[cg.scores[i].client].team!=TEAM_RED) &
     		   (cgs.clientinfo[cg.scores[i].client].team!=TEAM_BLUE) &
          	   (cgs.clientinfo[cg.scores[i].client].team!=TEAM_SPECTATOR))	
  		{
     			client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].dfph,stats[client].efficiency);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].efficiency);
      		trap_FS_Write(str_out,strlen(str_out),out);
			for(j=1;j<10;j++) if (cg_weapons[j].weaponIcon) 
        		{
				Com_sprintf(tmp,sizeof(tmp)," <td>%i</td>",stats[client].weapon[j-1]);
  				trap_FS_Write(tmp,strlen(tmp),out);
   			};
      		Com_sprintf(tmp,sizeof(tmp)," <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>",stats[client].excellent,stats[client].kills,stats[client].killed,stats[client].frags[client]);
  			trap_FS_Write(tmp,strlen(tmp),out);
    		};
 		strcpy(str_out,"</table>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
   
	// Scores table 2 (frags per player)
	else if(strcmp(tag,"!--SH_ScoresTable2--")==0)
 	{
  		weapcount = 0;
  		for(i=2;i<10;i++)if ( cg_weapons[i].weaponIcon ) weapcount++;
    		numcols = cg.numScores+9;
		Com_sprintf(str_out,sizeof(str_out),"<table cols = %i border>\r\n<tr><th>Name</th><th>Time</th><th>Score</th><th>FR</th><th>Eff.</th></th>",numcols);
  		trap_FS_Write(str_out,strlen(str_out),out);
		for(i=0;i<cg.numScores;i++) if(cgs.clientinfo[cg.scores[i].client].team==TEAM_BLUE) 
    		{
      		client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
      		Com_sprintf(tmp,sizeof(tmp),"<th>%s</th>",HTMLName);
        		trap_FS_Write(tmp,strlen(tmp),out);
      	};
  		for(i=0;i<cg.numScores;i++) if(cgs.clientinfo[cg.scores[i].client].team==TEAM_RED) 
    		{
      		client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
      		Com_sprintf(tmp,sizeof(tmp),"<th>%s</th>",HTMLName);
        		trap_FS_Write(tmp,strlen(tmp),out);
      	};
  		for(i=0;i<cg.numScores;i++) 
   		if((cgs.clientinfo[cg.scores[i].client].team!=TEAM_RED) &
     		   (cgs.clientinfo[cg.scores[i].client].team!=TEAM_BLUE) &
          	   (cgs.clientinfo[cg.scores[i].client].team!=TEAM_SPECTATOR))	
    		{
      		client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
      		Com_sprintf(tmp,sizeof(tmp),"<th>%s</th>",HTMLName);
        		trap_FS_Write(tmp,strlen(tmp),out);
      	};

    		strcpy(str_out,"<th>Exc.</th><th>Kills</th><th>Dth.</th><th>Suic.</th></tr>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
		
		for(i=0;i<cg.numScores;i++) if(cgs.clientinfo[cg.scores[i].client].team==TEAM_BLUE) 
  		{
     			client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].dfph,stats[client].efficiency);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td>  <td>%i</td> <td>%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].efficiency);
      		for(j=0;j<cg.numScores;j++) if(cgs.clientinfo[cg.scores[j].client].team!=TEAM_SPECTATOR)  
        		{
				Com_sprintf(tmp,sizeof(tmp)," <td>%i</td>",stats[client].frags[cg.scores[j].client]);
    				strcat(str_out,tmp);
   			};
      		Com_sprintf(tmp,sizeof(tmp)," <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>",stats[client].excellent,stats[client].kills,stats[client].killed,stats[client].frags[client]);
	        	strcat(str_out,tmp);
  			trap_FS_Write(str_out,strlen(str_out),out);
    		};
 		for(i=0;i<cg.numScores;i++) if(cgs.clientinfo[cg.scores[i].client].team==TEAM_RED) 
  		{
     			client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].dfph,stats[client].efficiency);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td>  <td>%i</td> <td>%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].efficiency);
      		for(j=0;j<cg.numScores;j++) if(cgs.clientinfo[cg.scores[j].client].team!=TEAM_SPECTATOR)  
        		{
				Com_sprintf(tmp,sizeof(tmp)," <td>%i</td>",stats[client].frags[cg.scores[j].client]);
    				strcat(str_out,tmp);
   			};
      		Com_sprintf(tmp,sizeof(tmp)," <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>",stats[client].excellent,stats[client].kills,stats[client].killed,stats[client].frags[client]);
	        	strcat(str_out,tmp);
  			trap_FS_Write(str_out,strlen(str_out),out);
    		};
 
		for(i=0;i<cg.numScores;i++)
   		if((cgs.clientinfo[cg.scores[i].client].team!=TEAM_RED) &
     		   (cgs.clientinfo[cg.scores[i].client].team!=TEAM_BLUE) &
          	   (cgs.clientinfo[cg.scores[i].client].team!=TEAM_SPECTATOR))	
  		{
     			client = cg.scores[i].client;
        		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td> <td>%i</td> <td>%i.%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].dfph,stats[client].efficiency);
      		else Com_sprintf(str_out,sizeof(str_out),"<tr> <td>%s</td> <td>%i</td>  <td>%i</td> <td>%i</td> <td>%i</td>",HTMLName,cg.scores[i].time,stats[client].score,stats[client].fph,stats[client].efficiency);
      		for(j=0;j<cg.numScores;j++) if(cgs.clientinfo[cg.scores[j].client].team!=TEAM_SPECTATOR)  
        		{
				Com_sprintf(tmp,sizeof(tmp)," <td>%i</td>",stats[client].frags[cg.scores[j].client]);
    				strcat(str_out,tmp);
   			};
      		Com_sprintf(tmp,sizeof(tmp)," <td>%i</td> <td>%i</td> <td>%i</td> <td>%i</td> </tr>",stats[client].excellent,stats[client].kills,stats[client].killed,stats[client].frags[client]);
	        	strcat(str_out,tmp);
  			trap_FS_Write(str_out,strlen(str_out),out);
    		};
 

		strcpy(str_out,"</table>\r\n");
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
      
	else if(strcmp(tag,"!--SH_Team--")==0)
 	{
  		str_out[0]=0;
		if(cgs.clientinfo[cg.snap->ps.clientNum].team==TEAM_SPECTATOR) strcpy(str_out,"Spectator");
		if(cgs.clientinfo[cg.snap->ps.clientNum].team==TEAM_FREE) strcpy(str_out,"Free/No team");
		if(cgs.clientinfo[cg.snap->ps.clientNum].team==TEAM_RED) strcpy(str_out,"Red");
		if(cgs.clientinfo[cg.snap->ps.clientNum].team==TEAM_BLUE) strcpy(str_out,"Blue");
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
	else if(strcmp(tag,"!--SH_Version--")==0)
 	{
		strcpy(str_out,SHUD_VERSION);
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
   
	else if(strcmp(tag,"!--SH_MapName--")==0)
 	{
  		trap_FS_Write(cgs.mapname,strlen(cgs.mapname),out);
  	} 
   
	else if(strcmp(tag,"!--SH_StatusMsg--")==0)
 	{
		switch ( SHUDLog_Status ) {
		case SHUD_STATUS_NONE:
			str_out[0]=0;
			break;
		case SHUD_STATUS_MANUAL:
			if(level_end_time==-1) strcpy(str_out,"Game not finished. Stats may not be complete.");
   			else strcpy(str_out,"Manual call of SHUD_WriteLog."); 
			break;
		case SHUD_STATUS_FRAGLIMIT_HIT:
  			if(level_end_time==-1) strcpy(str_out,"Fraglimit hit.");
	    		else
       		{
         			i=(level_end_time-cgs.levelStartTime)/60000;
         			j=((level_end_time-cgs.levelStartTime)/1000)-i*60;
            		p=((level_end_time-cgs.levelStartTime)/10)-i*6000-j*100;
              		Com_sprintf(str_out,sizeof(str_out),"Fraglimit hit after %im%02i.%i.",i,j,p);
            	};
			break;
		case SHUD_STATUS_TIMELIMIT_HIT:
			strcpy(str_out,"Timelimit hit.");
			break;
		case SHUD_STATUS_CAPTURELIMIT_HIT:
			
  			if(level_end_time==-1) strcpy(str_out,"Capturelimit hit.");
	    		else
       		{
         			i=(level_end_time-cgs.levelStartTime)/60000;
         			j=((level_end_time-cgs.levelStartTime)/1000)-i*60;
            		p=((level_end_time-cgs.levelStartTime)/10)-i*6000-j*100;
              		Com_sprintf(str_out,sizeof(str_out),"Capturelimit hit after %im%02i.%i.",i,j,p);
            	};
			break;
		case SHUD_STATUS_WARMUP:
			strcpy(str_out,"Warmup in progress. Stats may not be complete.");
			break;
		default:
			strcpy(str_out,"Unknown message.");
			break;
   		};
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
   
   
	else if(strcmp(tag,"!--SH_GameType--")==0)
 	{

		switch ( cgs.gametype ) {
		case GT_FFA:
			strcpy(str_out,"Free For All");
			break;
		case GT_SINGLE_PLAYER:
			strcpy(str_out,"Single Player");
			break;
		case GT_TOURNAMENT:
			strcpy(str_out,"Tournament");
			break;
		case GT_TEAM:
			strcpy(str_out,"Team Deathmatch");
			break;
		case GT_CTF:
			strcpy(str_out,"Capture The Flag");
			break;
		default:
			strcpy(str_out,"Unknown Gametype");
			break;
   		};
  		trap_FS_Write(str_out,strlen(str_out),out);
  	} 
   

	// Cvar output
	else if(CG_SHUDTagWithParm(tag,"SH_Cvar",parm))
	{	
 		trap_Cvar_VariableStringBuffer( parm, tmp, 256);
   		if(strcmp(parm,"name")==0) 
     		{
       		CG_SHUDConvertFunNameToHTML(tmp,HTMLName);
         		strcpy(tmp,HTMLName);
         	};
		trap_FS_Write( tmp, strlen(tmp), out );	
 	} 

	// Accuracy : shots fired
	else if(CG_SHUDTagWithParm(tag,"SH_Acc_Fired",parm))
	{	
		i = atoi(parm);
  		if(i==0) Com_sprintf(str_out,sizeof(str_out),"%i",cg.snap->ps.persistant[PERS_ACCURACY_SHOTS]);
    		else if ((i>=0)&(i<MAX_WEAPONS))
      	{
       		Com_sprintf(str_out,sizeof(str_out),"%i",acc_weap_f[i]);	
       	};
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 
  
	// Accuracy : hits
	else if(CG_SHUDTagWithParm(tag,"SH_Acc_Hits",parm))
	{	
		i = atoi(parm);
  		if(i==0) Com_sprintf(str_out,sizeof(str_out),"%i",cg.snap->ps.persistant[PERS_ACCURACY_HITS]);
    		else if ((i>=0)&(i<MAX_WEAPONS))
      	{
       		Com_sprintf(str_out,sizeof(str_out),"%i",acc_weap_h[i]);	
       	};
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Accuracy : max. consecutive hits
	else if(CG_SHUDTagWithParm(tag,"SH_Acc_MaxConHits",parm))
	{	
		i = atoi(parm);
  		if(i==0) Com_sprintf(str_out,sizeof(str_out),"%i",max_cons_hits_all);
    		else if ((i>=0)&(i<MAX_WEAPONS))
      	{
       		Com_sprintf(str_out,sizeof(str_out),"%i",max_cons_hits[i]);	
       	};
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Accuracy percentage
	else if(CG_SHUDTagWithParm(tag,"SH_Acc_Percent",parm))
	{	
		i = atoi(parm);
  		if(i==0) 
    		{
	       	if(cg.snap->ps.persistant[PERS_ACCURACY_SHOTS]==0) {p=0;pf=0;}
      	   	else
            	{
	         		p = (cg.snap->ps.persistant[PERS_ACCURACY_HITS]*100)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS];
      	   		pf = ((cg.snap->ps.persistant[PERS_ACCURACY_HITS]*1000)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS])-p*10;
	            };
      	 	if(pf!=0) Com_sprintf(str_out,sizeof(str_out),"%i.%i",p,pf);
         		else Com_sprintf(str_out,sizeof(str_out),"%i",p);
      	} else if ((i>0)&(i<MAX_WEAPONS))
      	{
	       	if(acc_weap_f[i]==0) {p=0;pf=0;}
      	   	else
            	{
	         		p = (acc_weap_h[i]*100)/acc_weap_f[i];
      	   		pf = ((acc_weap_h[i]*1000)/acc_weap_f[i])-p*10;
	            };
      	 	if(pf!=0) Com_sprintf(str_out,sizeof(str_out),"%i.%i",p,pf);
         		else Com_sprintf(str_out,sizeof(str_out),"%i",p);
       	};
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 
  
	// Player frag rate
	else if(strcmp(tag,"!--SH_FragRate--")==0)
	{	
 		if(cg.snap->ps.persistant[PERS_SCORE]!=0)
   		{
     			client = cg.snap->ps.clientNum;
     			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"%i.%i",stats[client].fph,stats[client].dfph);
      		else Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].fph);
   		} else strcpy(str_out,"0");
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Player max speed
	else if(strcmp(tag,"!--SH_MaxSpeed--")==0)
	{	
		Com_sprintf(str_out,sizeof(str_out),"%f",max_xyspeed);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player average health
	else if(strcmp(tag,"!--SH_AvgHealth--")==0)
	{	
		Com_sprintf(str_out,sizeof(str_out),"%i",avg_health);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player average armor
	else if(strcmp(tag,"!--SH_AvgArmor--")==0)
	{	
		Com_sprintf(str_out,sizeof(str_out),"%i",avg_armor);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player armor picked up
	else if(strcmp(tag,"!--SH_TotalArmor--")==0)
	{	
		Com_sprintf(str_out,sizeof(str_out),"%i",armor_picked_up);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player efficiency
	else if(strcmp(tag,"!--SH_Efficiency--")==0)
	{	
     		client = cg.snap->ps.clientNum;
		Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].efficiency);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player score
	else if(strcmp(tag,"!--SH_Score--")==0)
	{	
     		client = cg.snap->ps.clientNum;
		Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].score);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 
  
	// Player time
	else if(strcmp(tag,"!--SH_TimeInGame--")==0)
	{	
		for ( i = 0 ; i < cg.numScores ; i++ ) if ( cg.scores[i].client == cg.snap->ps.clientNum ) 
		Com_sprintf(str_out,sizeof(str_out),"%i",cg.scores[i].time);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 
  
	// Server time
	else if(strcmp(tag,"!--SH_ServerTime--")==0)
	{	
         	i=(cg.time-cgs.levelStartTime)/60000;
         	j=((cg.time-cgs.levelStartTime)/1000)-i*60;
            p=((cg.time-cgs.levelStartTime)/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 


	// Player kills
	else if(strcmp(tag,"!--SH_Kills--")==0)
	{	
     		client = cg.snap->ps.clientNum;
		Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].kills);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player deaths
	else if(strcmp(tag,"!--SH_Deaths--")==0)
	{	
     		client = cg.snap->ps.clientNum;
		Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].killed);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player suicides
	else if(strcmp(tag,"!--SH_Suicides--")==0)
	{	
     		client = cg.snap->ps.clientNum;
		Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].frags[client]);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 


	// Player award : impressive
	else if(strcmp(tag,"!--SH_Impressive--")==0)
	{	
		Com_sprintf(str_out,sizeof(str_out),"%i",cg.snap->ps.persistant[PERS_IMPRESSIVE_COUNT]);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player award : excellent
	else if(strcmp(tag,"!--SH_Excellent--")==0)
	{	
		Com_sprintf(str_out,sizeof(str_out),"%i",cg.snap->ps.persistant[PERS_EXCELLENT_COUNT]);
  		trap_FS_Write(str_out,strlen(str_out),out);
 	} 

	// Player times under powerups
	else if(CG_SHUDTagWithParm(tag,"SH_TimeUnderPowerup",parm))
	{	
 		k=atoi(parm);
   		if((i>=0)&(i<MAX_POWERUPS))
     		{
         		i=time_under_powerup[k]/60000;
         		j=(time_under_powerup[k]/1000)-i*60;
            	p=(time_under_powerup[k]/10)-i*6000-j*100;
              	Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
  		} else strcpy(str_out,"SH_TimeUnderPowerup : wrong powerup reference.");
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 

	// Player domination times
	else if(strcmp(tag,"!--SH_LeadingTime--")==0)
	{	
         	i=lead_time/60000;
         	j=(lead_time/1000)-i*60;
            p=(lead_time/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_TiedTime--")==0)
	{	
         	i=equal_time/60000;
         	j=(equal_time/1000)-i*60;
            p=(equal_time/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_LosingTime--")==0)
	{	
          	i=lose_time/60000;
         	j=(lose_time/1000)-i*60;
            p=(lose_time/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 

	// Player domination times (percent)
	else if(strcmp(tag,"!--SH_LeadingPercentTime--")==0)
	{	
   		if(level_end_time==-1) j = (cg.time-cgs.levelStartTime);
     		else j = (level_end_time-cgs.levelStartTime);
            p=(lead_time*100/j);
            Com_sprintf(str_out,sizeof(str_out),"%i",p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_TiedPercentTime--")==0)
	{	
   		if(level_end_time==-1) j = (cg.time-cgs.levelStartTime);
     		else j = (level_end_time-cgs.levelStartTime);
            p =(equal_time*100/j);
            Com_sprintf(str_out,sizeof(str_out),"%i",p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_LosingPercentTime--")==0)
	{	
   		if(level_end_time==-1) j = (cg.time-cgs.levelStartTime);
     		else j = (level_end_time-cgs.levelStartTime);
            p=(lose_time*100/j);
            Com_sprintf(str_out,sizeof(str_out),"%i",p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 


	// Team domination times
	else if(strcmp(tag,"!--SH_TeamBlueLeadingTime--")==0)
	{	
         	i=team_blead_time/60000;
         	j=(team_blead_time/1000)-i*60;
            p=(team_blead_time/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_TeamTiedTime--")==0)
	{	
         	i=team_equal_time/60000;
         	j=(team_equal_time/1000)-i*60;
            p=(team_equal_time/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_TeamRedLeadingTime--")==0)
	{	
          	i=team_rlead_time/60000;
         	j=(team_rlead_time/1000)-i*60;
            p=(team_rlead_time/10)-i*6000-j*100;
            Com_sprintf(str_out,sizeof(str_out),"%im%02i.%i.",i,j,p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 

	// Team domination times (percent)
	else if(strcmp(tag,"!--SH_TeamBlueLeadingPercentTime--")==0)
	{	
   		if(level_end_time==-1) j = (cg.time-cgs.levelStartTime);
     		else j = (level_end_time-cgs.levelStartTime);
            p=(team_blead_time*100/j);
            Com_sprintf(str_out,sizeof(str_out),"%i",p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_TeamTiedPercentTime--")==0)
	{	
   		if(level_end_time==-1) j = (cg.time-cgs.levelStartTime);
     		else j = (level_end_time-cgs.levelStartTime);
            p =(team_equal_time*100/j);
            Com_sprintf(str_out,sizeof(str_out),"%i",p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 
	else if(strcmp(tag,"!--SH_TeamRedLeadingPercentTime--")==0)
	{	
   		if(level_end_time==-1) j = (cg.time-cgs.levelStartTime);
     		else j = (level_end_time-cgs.levelStartTime);
            p=(team_rlead_time*100/j);
            Com_sprintf(str_out,sizeof(str_out),"%i",p);
    		trap_FS_Write( str_out, strlen(str_out), out );
 	} 

	// CTF : flags taken
	else if(CG_SHUDTagWithParm(tag,"SH_CTFAttacks",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<=1))
     		{
       		if (i==1) j = teamred_getflag;
       		if (i==0) j = teamblue_getflag;       
     			Com_sprintf(str_out,sizeof(str_out),"%i",j);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_CTFAttacks : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// CTF : flags retrieved
	else if(CG_SHUDTagWithParm(tag,"SH_CTFRetrieved",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<=1))
     		{
       		if (i==1) j = teamred_getbacks;
       		if (i==0) j = teamblue_getbacks;       
     			Com_sprintf(str_out,sizeof(str_out),"%i",j);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_CTFRetrieved : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Kills per weapon (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_KillsWeapon",parm))
	{		
 		ptr = strchr(parm,',');
   		if(!ptr)
     		{
       		client = cg.snap->ps.clientNum;
       		j=atoi(parm);
              	if((j>=0)&(j<MAX_WEAPONS))
               	{
         			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].weapon[j]);	
       		} else Com_sprintf(str_out,sizeof(str_out), "SH_KillsWeapon : incorrect parameter (weapon %j)",j);
       	} else
        	{
         		ptr[0]=0;
           		i=atoi(parm);
             	j=atoi(&ptr[1]);
              	if((j>=0)&(j<MAX_WEAPONS))
               	{
   				if((i>=0)&(i<MAX_CLIENTS))
     				{
     					client = cg.scores[i].client;
   					Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].weapon[j]);
   				} else Com_sprintf(str_out,sizeof(str_out), "SH_KillsWeapon : incorrect parameter (player %i)",i);
       		} else Com_sprintf(str_out,sizeof(str_out), "SH_KillsWeapon : incorrect parameter (weapon %j)",j);
      	};
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 


	// Kills per player (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_KillsPlayer",parm))
	{		
 		ptr = strchr(parm,',');
   		if(!ptr)
     		{
       		client = cg.snap->ps.clientNum;
       		j=atoi(parm);
              	if((j>=0)&(j<MAX_CLIENTS))
               	{
     				j = cg.scores[j].client;
         			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].frags[j]);	
       		} else Com_sprintf(str_out,sizeof(str_out), "SH_KillsPlayer : incorrect parameter (weapon %j)",j);
       	} else
        	{
         		ptr[0]=0;
           		i=atoi(parm);
             	j=atoi(&ptr[1]);
              	if((j>=0)&(j<MAX_CLIENTS))
               	{
   				if((i>=0)&(i<MAX_CLIENTS))
     				{
     					client = cg.scores[i].client;
     					j = cg.scores[j].client;
   					Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].frags[j]);
   				} else Com_sprintf(str_out,sizeof(str_out), "SH_KillsPlayer : incorrect parameter (player %i)",i);
       		} else Com_sprintf(str_out,sizeof(str_out), "SH_KillsPlayer : incorrect parameter (weapon %j)",j);
      	};
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 


	// Frag rate (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_FragRate",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			if(stats[client].dfph!=0) Com_sprintf(str_out,sizeof(str_out),"%i.%i",stats[client].fph,stats[client].dfph);
      		else Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].fph);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_FragRate : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Excellent (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_Excellent",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].excellent);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_Excellent : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Efficiency (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_Efficiency",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].efficiency);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_Efficiency : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Scores (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_Score",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].score);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_Score : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 
  
	// Kills (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_Kills",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].kills);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_Kills : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Deaths (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_Deaths",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].killed);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_Deaths : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Suicides (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_Suicides",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
     			client = cg.scores[i].client;
   			Com_sprintf(str_out,sizeof(str_out),"%i",stats[client].frags[client]);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_Suicides : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Names (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_PlayerName",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
       		client = cg.scores[i].client;
          		CG_SHUDConvertFunNameToHTML(cgs.clientinfo[client].name,HTMLName);
			strcpy(str_out,HTMLName);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_PlayerName : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 

	// Times (all players)
	else if(CG_SHUDTagWithParm(tag,"SH_TimeInGame",parm))
	{	
 		i=atoi(parm);
   		if((i>=0)&(i<MAX_CLIENTS))
     		{
   			Com_sprintf(str_out,sizeof(str_out),"%i",cg.scores[i].time);
   		} else Com_sprintf(str_out,sizeof(str_out), "SH_TimeInGame : incorrect parameter (%i)",i);
		trap_FS_Write( str_out, strlen(str_out), out );	
 	} 


      else // not a SHUD tag, simply copy it
   	{
    		trap_FS_Write( "<", 1, out );	
    		trap_FS_Write( tag, strlen(tag), out );
    		trap_FS_Write( ">", 1, out );	
     	};

}

void CG_SHUDLog( void ) {

	fileHandle_t in,out;
 	int template_length,file_length,length,total_length,tag_length;
    	char tag[256];
      char ch_get;
 	qboolean ok;

	template_length = trap_FS_FOpenFile(cg_SHUDLogTemplate.string, &in, FS_READ );	

	if(in) // Template found
 	{
  		file_length = trap_FS_FOpenFile(cg_SHUDLogFile.string, &out, FS_APPEND );
    		if(out)
      	{
       		total_length = template_length;
         		ok=qtrue;
         		while((total_length>0) & (ok))
           		{
         			trap_FS_Read( &ch_get, 1, in ); total_length--;
				if(ch_get!='<') trap_FS_Write( &ch_get, 1, out );
    				else // Tag found 
        			{
           				memset(tag,0,256);	
                  		tag_length=0;
                    		while((tag_length<256) && (ch_get!='>'))
                      		{
	         				trap_FS_Read( &ch_get, 1, in ); total_length--;
              				if(ch_get!='>') {tag[tag_length] = ch_get; tag_length++;};
                  			if(tag_length==256) 
                     			{
                        			trap_Print("SHUD error : Tag too long (256 chars max).\n");
                           			ok = qfalse;
                              	};
             			}
                			CG_SHUDConvertTag(tag,out); // Test tag and write it.
           			}
			}
           		trap_FS_FCloseFile(out);
	            if(ok) trap_Print("SHUD : Log file creation successful.\n");
             	else trap_Print("SHUD : Error while log file generation.\n"); 
       	} else trap_Print("SHUD Error : Could not open log file for writing.\n");
		trap_FS_FCloseFile(in);  	
  	} else trap_Print("SHUD Error : HTML log template not found.\n");

}
