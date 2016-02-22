/* db_new.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include "merc.h"
#include "obj.h"
#include "db.h"


/* values for db2.c */
struct		social_type	social_table		[MAX_SOCIALS];
int		social_count		= 0;

/* snarf a socials file */
void load_socials( FILE *fp)
{
    for ( ; ; ) 
    {
    	struct social_type social;
    	char *temp;
        /* clear social */
	social.char_no_arg = NULL;
	social.others_no_arg = NULL;
	social.char_found = NULL;
	social.others_found = NULL;
	social.vict_found = NULL; 
	social.char_not_found = NULL;
	social.char_auto = NULL;
	social.others_auto = NULL;

    	temp = fread_word(fp);
    	if (!strcmp(temp,"#0"))
	    return;  /* done */

    	strcpy(social.name,temp);
    	fread_to_eol(fp);

	temp = fread_string_eol(fp);
	if (!strcmp(temp,"$"))
	     social.char_no_arg = NULL;
	else if (!strcmp(temp,"#"))
	{
	     social_table[social_count] = social;
	     social_count++;
	     continue; 
	}
        else
	    social.char_no_arg = temp;

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.others_no_arg = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_no_arg = temp;

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.char_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
       	else
	    social.char_found = temp;

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.others_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_found = temp; 

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.vict_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.vict_found = temp;

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.char_not_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.char_not_found = temp;

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.char_auto = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.char_auto = temp;
         
        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.others_auto = NULL;
        else if (!strcmp(temp,"#"))
        {
             social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_auto = temp; 
	
	social_table[social_count] = social;
    	social_count++;
   }
   return;
}
    





/*
 * Snarf a mob section.  new style
 */
void load_mobiles( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;
 
    for ( ; ; )
    {
        sh_int vnum;
        char letter,temp;
        int iHash;
 
        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_mobiles: # not found.", 0 );
            exit( 1 );
        }
 
        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;
 
        fBootDb = FALSE;
        if ( get_mob_index( vnum ) != NULL )
        {
            bug( "Load_mobiles: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;
 
        pMobIndex                       = alloc_perm( sizeof(*pMobIndex) );
        pMobIndex->vnum                 = vnum;
	pMobIndex->new_format		= TRUE;
	newmobs++;
        pMobIndex->player_name          = fread_string( fp );
        pMobIndex->short_descr          = fread_string( fp );
        pMobIndex->long_descr           = fread_string( fp );
        pMobIndex->description          = fread_string( fp );
	pMobIndex->race		 	= race_lookup(fread_string( fp ));
 
        pMobIndex->long_descr[0]        = UPPER(pMobIndex->long_descr[0]);
        pMobIndex->description[0]       = UPPER(pMobIndex->description[0]);
 
        pMobIndex->act                  = fread_flag( fp ) | ACT_IS_NPC
					| race_table[pMobIndex->race].act;
        pMobIndex->affected_by          = fread_flag( fp )
					| race_table[pMobIndex->race].aff;
        pMobIndex->pShop                = NULL;
        pMobIndex->alignment            = fread_number( fp );
        letter                          = fread_letter( fp );

        pMobIndex->level                = fread_number( fp );
        pMobIndex->hitroll              = fread_number( fp );  

	/* read hit dice */
        pMobIndex->hit[DICE_NUMBER]     = fread_number( fp );  
        /* 'd'          */                fread_letter( fp ); 
        pMobIndex->hit[DICE_TYPE]   	= fread_number( fp );
        /* '+'          */                fread_letter( fp );   
        pMobIndex->hit[DICE_BONUS]      = fread_number( fp ); 

 	/* read mana dice */
	pMobIndex->mana[DICE_NUMBER]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->mana[DICE_TYPE]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->mana[DICE_BONUS]	= fread_number( fp );

	/* read damage dice */
	pMobIndex->damage[DICE_NUMBER]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->damage[DICE_TYPE]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->damage[DICE_BONUS]	= fread_number( fp );
	pMobIndex->dam_type		= fread_number( fp );

	/* read armor class */
	pMobIndex->ac[AC_PIERCE]	= fread_number( fp ) * 10;
	pMobIndex->ac[AC_BASH]		= fread_number( fp ) * 10;
	pMobIndex->ac[AC_SLASH]		= fread_number( fp ) * 10;
	pMobIndex->ac[AC_EXOTIC]	= fread_number( fp ) * 10;

	/* read flags and add in data from the race table */
	pMobIndex->off_flags		= fread_flag( fp ) 
					| race_table[pMobIndex->race].off;
	pMobIndex->imm_flags		= fread_flag( fp )
					| race_table[pMobIndex->race].imm;
	pMobIndex->res_flags		= fread_flag( fp )
					| race_table[pMobIndex->race].res;
	pMobIndex->vuln_flags		= fread_flag( fp )
					| race_table[pMobIndex->race].vuln;

	/* vital statistics */
	pMobIndex->start_pos		= fread_number( fp );
	pMobIndex->default_pos		= fread_number( fp );
	pMobIndex->sex			= fread_number( fp );
	pMobIndex->gold			= fread_number( fp );

	pMobIndex->form			= fread_flag( fp )
					| race_table[pMobIndex->race].form;
	pMobIndex->parts		= fread_flag( fp )
					| race_table[pMobIndex->race].parts;
	/* size */
	temp				= fread_letter( fp );
	switch (temp)
	{
	    case ('T') :		pMobIndex->size = SIZE_TINY;	break;
	    case ('S') :		pMobIndex->size = SIZE_SMALL;	break;
	    case ('M') :		pMobIndex->size = SIZE_MEDIUM;	break;
	    case ('L') :		pMobIndex->size = SIZE_LARGE; 	break;
	    case ('H') :		pMobIndex->size = SIZE_HUGE;	break;
	    case ('G') :		pMobIndex->size = SIZE_GIANT;	break;
	    default:			pMobIndex->size = SIZE_MEDIUM; break;
	}
	pMobIndex->material		= material_lookup(fread_word( fp ));
 
        if ( letter != 'S' )
        {
            bug( "Load_mobiles: vnum %d non-S.", vnum );
            exit( 1 );
        }
 
        iHash                   = vnum % MAX_KEY_HASH;
        pMobIndex->next         = mob_index_hash[iHash];
        mob_index_hash[iHash]   = pMobIndex;
        top_mob_index++;
        kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
    }
 
    return;
}

/*
 * Snarf an obj section. new style
 */
void load_objects( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;
 
    for ( ; ; )
    {
        sh_int vnum;
        char letter;
        int iHash;
 
        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_objects: # not found.", 0 );
            exit( 1 );
        }
 
        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;
 
        fBootDb = FALSE;
        if ( get_obj_index( vnum ) != NULL )
        {
            bug( "Load_objects: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;
 
        pObjIndex                       = alloc_perm( sizeof(*pObjIndex) );
        pObjIndex->vnum                 = vnum;
	pObjIndex->reset_num		= 0;
        pObjIndex->name                 = fread_string( fp );
        pObjIndex->short_descr          = fread_string( fp );
        pObjIndex->description          = fread_string( fp );
        pObjIndex->material		= material_lookup(fread_string( fp ));
 
        pObjIndex->type            	= fread_flag( fp );
        pObjIndex->extra_flags          = fread_flag( fp );
        pObjIndex->wear_flags           = fread_flag( fp );
	pObjIndex->level		= fread_number( fp );
        pObjIndex->weight               = fread_number( fp );
        pObjIndex->cost                 = fread_number( fp ); 

        /* condition */
        letter 				= fread_letter( fp );
	switch (letter)
 	{
	    case ('P') :		pObjIndex->condition = 100; break;
	    case ('G') :		pObjIndex->condition =  90; break;
	    case ('A') :		pObjIndex->condition =  75; break;
	    case ('W') :		pObjIndex->condition =  50; break;
	    case ('D') :		pObjIndex->condition =  25; break;
	    case ('B') :		pObjIndex->condition =  10; break;
	    case ('R') :		pObjIndex->condition =   0; break;
	    default:			pObjIndex->condition = 100; break;
	}
 
        for ( ; ; )
        {
            char letter;
 
            letter = fread_letter( fp );
 
            if ( letter == 'A' )
            {
                AFFECT_DATA *paf;
 
                paf                     = alloc_perm( sizeof(*paf) );
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                paf->location           = fread_number( fp );
                paf->modifier           = fread_number( fp );
                paf->bitvector          = 0;
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
            }
 
            else if ( letter == 'E' )
            {
                EXTRA_DESCR_DATA *ed;
 
                ed                      = alloc_perm( sizeof(*ed) );
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
                ed->next                = pObjIndex->extra_descr;
                pObjIndex->extra_descr  = ed;
                top_ed++;
            }

	    else if ( letter == 'W' ) /* add a weapon field */
	    {
		WEAPON_DATA *weapon;

		if (!IS_SET(pObjIndex->type,ITEM_WEAPON))
		    bug("Load_objects: vnum %d has weapon field, no bit",vnum);
		
		weapon			    = alloc_perm(sizeof(*weapon));
		weapon->blade		    = material_lookup(fread_string(fp));
		weapon->slaying		    = fread_string(fp);
		weapon->type		    = fread_number(fp);
		weapon->message		    = fread_number(fp);
		weapon->flags		    = fread_flag(fp);
		weapon->hit		    = fread_number(fp);
		weapon->damage[DICE_NUMBER] = fread_number(fp);
		/* d */			      fread_letter(fp);
		weapon->damage[DICE_TYPE]   = fread_number(fp);
		/* + */			      fread_letter(fp);
		weapon->damage[DICE_BONUS]  = fread_number(fp);

		pObjIndex->weapon	    = weapon;
		top_weapon++;
	    }

	    else if ( letter == 'R' ) /* add an armor field */
	    {
		ARMOR_DATA *armor;
		int i;

		if (!IS_SET(pObjIndex->type,ITEM_ARMOR))
		    bug("Load_objects: vnum %d has armor field, no bit",vnum);
		
		armor			= alloc_perm(sizeof(*armor));
		for (i = 0; i < 4; i ++)
		    armor->ac[i]	= fread_number(fp);
		armor->bonus		= fread_number(fp);
		armor->bulk		= fread_number(fp);

		pObjIndex->armor	= armor;
		top_armor++;
	    }

	    else if ( letter == 'C' ) /* add a container field */
	    {
		CONT_DATA *cont;

		if (!IS_SET(pObjIndex->type,ITEM_CONTAINER))
		    bug("Load_objects: vnum %d has cont field, no bit",vnum);

		if (IS_SET(pObjIndex->type,ITEM_DRINK_CON))
		    bug("Load_objects: vnum %d is both drink and cont",vnum);

		cont			= alloc_perm(sizeof(*cont));
		cont->closed		= fread_flag(fp);
		cont->key		= fread_number(fp);
		cont->volume		= fread_number(fp);
		cont->limit		= fread_number(fp);

		pObjIndex->cont		= cont;
		top_cont++;
	    }

	    else if ( letter == 'D' ) /* add a drink field */
	    {
		CONT_DATA *cont;
		
		if (!IS_SET(pObjIndex->type,ITEM_DRINK_CON))
		    bug("Load_objects: vnum %d has dcon field, no bit",vnum);

		if (IS_SET(pObjIndex->type,ITEM_CONTAINER))
		    bug("Load_objects: vnum %d is both drink and cont",vnum);

                cont                    = alloc_perm(sizeof(*cont));
		cont->liquid		= fread_number(fp);
		cont->volume		= fread_number(fp);
		cont->limit		= fread_number(fp);
		cont->poison		= fread_number(fp);

		pObjIndex->cont		= cont;
		top_cont++;
	    }

	    else if ( letter == 'F' ) /* add a food field */
	    {
		FOOD_DATA *food;

		if (!IS_SET(pObjIndex->type,ITEM_FOOD))
		    bug("Load_objects: vnum %d has food field, no bit",vnum);

		food			= alloc_perm(sizeof(*food));
		food->bite		= fread_number(fp);
		food->full		= fread_number(fp);
		food->taste		= fread_number(fp);
		food->poison		= fread_number(fp);

		pObjIndex->food		= food;
		top_food++;
	    }

	    else if ( letter == 'S' ) /* add a spell field */
	    {
		SPELL_DATA *spells;
		int sum,i;
		
		sum = IS_SET(pObjIndex->type,ITEM_SCROLL) + 
		      IS_SET(pObjIndex->type,ITEM_PILL) +
		      IS_SET(pObjIndex->type,ITEM_POTION);

		if ( sum == 0 ) /* all false */
		    bug("Load_objects: vnum %d has spell field, no bit",vnum);

		if ( sum > 1 ) /* more than one true */
		    bug("Load_objects: vnum %d has more than one spell field",
			 vnum);

		spells			= alloc_perm(sizeof(*spells));
		spells->level		= fread_number(fp);
		spells->class		= class_lookup(fread_word(fp));
		spells->delay		= fread_number(fp);

		for (i = 0; i < 4; i++ )
		    spells->spell[i]	= skill_lookup(fread_word(fp));

		pObjIndex->spells	= spells;
		top_spells++;
	    }

	    else if ( letter == 'U' ) /* add a use field */
	    {
		USE_DATA *use;
		int sum;

		sum = IS_SET(pObjIndex->type,ITEM_WAND) +
		      IS_SET(pObjIndex->type,ITEM_STAFF);

                if ( sum == 0 ) /* all false */
                    bug("Load_objects: vnum %d has use field, no bit",vnum);
 
                if ( sum > 1 ) /* more than one true */
                    bug("Load_objects: vnum %d has more than one use field",
                         vnum);


		use 			= alloc_perm(sizeof(*use));
		use->level 		= fread_number(fp);
		use->class		= class_lookup(fread_word(fp));
		use->delay		= fread_number(fp);
		use->charges		= fread_number(fp);
		use->max		= fread_number(fp);
		use->spell		= skill_lookup(fread_word(fp));

		pObjIndex->use		= use;
		top_use++;
	    }

	    else if ( letter == 'L' ) /* add a light field */
	    {
		LIGHT_DATA *light;
		
		if (!IS_SET(pObjIndex->type,ITEM_LIGHT))
		    bug("Load_objects: vnum %d has light field, not bit",vnum);

		light			= alloc_perm(sizeof(*light));
		light->duration		= fread_number(fp);
		light->max_duration	= fread_number(fp);
		light->fuel		= fread_number(fp);
		light->intensity	= fread_number(fp);

		pObjIndex->light	= light;
		top_light++;
	    }
		
		
 
            else
            {
                ungetc( letter, fp );
                break;
            }
        }

        iHash                   = vnum % MAX_KEY_HASH;
        pObjIndex->next         = obj_index_hash[iHash];
        obj_index_hash[iHash]   = pObjIndex;
        top_obj_index++;
    }
 
    return;
}

/* handling code for the object fields. Each field has a new, free, and copy */


/* new commands -- generate a new occurence of a field */
WEAPON_DATA *new_weapon( void )
{
    WEAPON_DATA *weapon;

    if (weapon_free == NULL)
    {
	top_weapon++;
	weapon = alloc_perm(sizeof(*weapon));
    }
    else
    {
	weapon 		= weapon_free;
	weapon_free	= weapon_free->next;
	weapon->next	= NULL;
    }

    return weapon;
}

ARMOR_DATA *new_armor(void)
{
    ARMOR_DATA *armor;

    if (armor_free == NULL)
    {
	top_armor++;
	armor = alloc_perm(sizeof(*armor));
    }
    else
    {
	armor		= armor_free;
	armor_free	= armor_free->next;
	armor->next	= NULL;
    }

    return armor;
}

CONT_DATA *new_cont(void)
{
    CONT_DATA *cont;

    if (cont_free == NULL)
    {
	top_cont++;
	cont = alloc_perm(sizeof(*cont));
    }
    else
    {
	cont		= cont_free;
	cont_free	= cont_free->next;
	cont->next	= NULL;
    }

    return cont;
}

FOOD_DATA *new_food(void)
{
    FOOD_DATA *food;

    if (food_free == NULL)
    {
	top_food++;
	food = alloc_perm(sizeof(*food));
    }
    else
    {
	food		= food_free;
	food_free	= food_free->next;
	food->next	= NULL;
    }

    return food;
}

SPELL_DATA *new_spells(void)
{
    SPELL_DATA *spells;
    int i;

    if (spell_free == NULL)
    {
	top_spells++;
	spells = alloc_perm(sizeof(*spells));
	for (i = 0; i < 4; i++)
	    spells->spell[i] = -1;
    }
    else
    {
	spells		= spell_free;
	spell_free	= spell_free->next;
	spells->next	= NULL;
    }

    return spells;
}

USE_DATA *new_use(void)
{
    USE_DATA *use;

    if (use_free == NULL)
    {
	top_use++;
	use = alloc_perm(sizeof(*use));
	use->spell = -1;
    }
    else
    {
	use		= use_free;
	use_free	= use_free->next;
	use->next	= NULL;
    }

    return use;
}

LIGHT_DATA *new_light(void)
{
    LIGHT_DATA *light;

    if (light_free == NULL)
    {
	top_light++;
	light = alloc_perm(sizeof(*light));
    }
    else
    {
	light		= light_free;
	light_free	= light_free->next;
	light->next	= NULL;
    }

    return light;
}


/* free code for fields -- puts the field on the free list, and clears it */
void free_weapon (OBJ_DATA *obj)
{
    int i;

    if (obj->weapon == NULL)
	return;
  
    if (obj->weapon == obj->pIndexData->weapon)
    {
	obj->weapon = NULL;
	return;
    }

    obj->weapon->type 		= 0;
    obj->weapon->message	= 0;
    obj->weapon->blade		= 0;
    obj->weapon->flags		= 0;
    obj->weapon->hit		= 0;
     
    for (i = 0; i < 3; i++)
	obj->weapon->damage[i]	= 0;

    free_string(obj->weapon->slaying);

    obj->weapon->next 	= weapon_free;
    weapon_free		= obj->weapon;
    obj->weapon		= NULL;
}

void free_armor (OBJ_DATA *obj)
{
    int i;

    if (obj->armor == NULL)
	return;

    if (obj->armor == obj->pIndexData->armor)
    {	
	obj->armor = NULL;
	return;
    }

    for (i = 0; i < 4; i++)
	obj->armor->ac[i]	= 0;

    obj->armor->bonus	= 0;
    obj->armor->bulk	= 0;

    obj->armor->next	= armor_free;
    armor_free		= obj->armor;
    obj->armor		= NULL;
}

void free_cont (OBJ_DATA *obj)
{

    if (obj->cont == NULL)
	return;

    if (obj->cont == obj->pIndexData->cont)
    {
	obj->cont = NULL;
	return;
    }

    obj->cont->closed	= 0;
    obj->cont->key	= 0;
    obj->cont->volume	= 0;
    obj->cont->limit	= 0;
    obj->cont->liquid	= 0;
    obj->cont->poison	= 0;

    obj->cont->next	= cont_free;
    cont_free		= obj->cont;
    obj->cont		= NULL;
}

void free_food (OBJ_DATA *obj)
{
    if (obj->food == NULL)
	return;

    if (obj->food == obj->pIndexData->food)
    {
	obj->food = NULL;
	return;
    }

    if (obj->food == obj->pIndexData->food)

    obj->food->full	= 0;
    obj->food->bite	= 0;
    obj->food->taste	= 0;
    obj->food->poison	= 0;

    obj->food->next	= food_free;
    food_free		= obj->food;
    obj->food		= NULL;
}

void free_spells (OBJ_DATA *obj)
{
    int i;

    if (obj->spells == NULL)
	return;

    if (obj->spells == obj->pIndexData->spells)
    {
	obj->spells = NULL;
	return;
    }

    obj->spells->level	= 0;
    obj->spells->class	= 0;
    obj->spells->delay	= 0;
    
    for (i = 0; i < 4; i++)
	obj->spells->spell[i]	= -1;

    obj->spells->next	= spell_free;
    spell_free		= obj->spells;
    obj->spells		= NULL;
}

void free_use (OBJ_DATA *obj)
{
    if (obj->use == NULL)
	return;

    if (obj->use == obj->pIndexData->use)
    {
	obj->use = NULL;
	return;
    }

    obj->use->level	= 0;
    obj->use->class	= 0;
    obj->use->delay	= 0;
    obj->use->charges	= 0;
    obj->use->max	= 0;
    obj->use->spell	= -1;

    obj->use->next	= use_free;
    use_free		= obj->use;
    obj->use		= NULL;
}

void free_light (OBJ_DATA *obj)
{
    if (obj->light == NULL)
	return;

    if (obj->light == obj->pIndexData->light)
    {
	obj->light = NULL;
	return;
    }

    obj->light->duration	= 0;
    obj->light->max_duration 	= 0;
    obj->light->fuel		= 0;
    obj->light->intensity	= 0;

    obj->light->next	= light_free;
    light_free		= obj->light;
    obj->light		= NULL;
}


/* copy code -- for copying fields */

WEAPON_DATA *copy_weapon (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->weapon == obj->pIndexData->weapon) 
    ||  (!clone && obj->weapon != obj->pIndexData->weapon)
    ||	obj->weapon == NULL)
	return obj->weapon;  /* shallow copy */

    else
    {
	WEAPON_DATA *weapon;
	int i;

	weapon 		= new_weapon();
	weapon->type	= obj->weapon->type;
	weapon->message	= obj->weapon->message;
	weapon->blade	= obj->weapon->blade;
	weapon->flags	= obj->weapon->flags;
	weapon->hit		= obj->weapon->hit;

    	for (i = 0; i < 3; i++)
            weapon->damage[i]       = obj->weapon->damage[i];
	
	weapon->slaying	= str_dup(obj->weapon->slaying);

	return weapon;
    }
}

ARMOR_DATA *copy_armor (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->armor == obj->pIndexData->armor)
    ||	(!clone && obj->armor != obj->pIndexData->armor)
    ||  obj->armor == NULL)	  
	return obj->armor; /* shallow copy */

     else
     {
	ARMOR_DATA *armor;
	int i;

	armor 		= new_armor();

	for (i = 0; i < 4; i++)
	    armor->ac[i]	= obj->armor->ac[i];

	armor->bonus	= obj->armor->bonus;
	armor->bulk	= obj->armor->bulk;

	return armor;
    }
}

CONT_DATA *copy_cont (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->cont == obj->pIndexData->cont)
    ||	(!clone && obj->cont != obj->pIndexData->cont)
    ||	obj->cont == NULL)
	return obj->cont; /* shallow copy */

    else
    {
	CONT_DATA *cont;

	cont		= new_cont();
	cont->closed	= obj->cont->closed;
	cont->key	= obj->cont->key;
	cont->volume	= obj->cont->volume;
	cont->limit	= obj->cont->limit;
	cont->liquid	= obj->cont->liquid;
	cont->poison	= obj->cont->poison;
    
	return cont;
    }
}

FOOD_DATA *copy_food (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->food == obj->pIndexData->food)
    ||	(!clone && obj->food != obj->pIndexData->food)
    ||	obj->cont == NULL)
	return obj->food; /* shallow copy */

    else
    {
	FOOD_DATA *food;

	food		= new_food();
	food->full	= obj->food->full;
	food->bite	= obj->food->bite;
	food->taste	= obj->food->taste;
	food->poison	= obj->food->poison;

	return food;
    }
}

SPELL_DATA *copy_spells (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->spells == obj->pIndexData->spells)
    || 	(!clone && obj->spells != obj->pIndexData->spells)
    ||	obj->spells == NULL)
	return obj->spells; /* shallow copy */

    else
    {
	SPELL_DATA *spells;
	int i;

	spells		= new_spells();
	spells->level	= obj->spells->level;
	spells->class	= obj->spells->class;
	spells->delay	= obj->spells->delay;

	for (i = 0; i < 4; i++)
	    spells->spell[i]	= obj->spells->spell[i];

	return spells;
    }
}

USE_DATA *copy_use (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->use == obj->pIndexData->use)
    ||	(!clone && obj->use != obj->pIndexData->use)
    ||	obj->use == NULL)
	return obj->use; /* shallow copy */

    else
    {
	USE_DATA *use;

	use		= new_use();
	use->level	= obj->use->level;
	use->class	= obj->use->class;
	use->delay	= obj->use->delay;
	use->charges	= obj->use->charges;
	use->max	= obj->use->max;
	use->spell	= obj->use->spell;

	return use;
    }
}

LIGHT_DATA *copy_light (const OBJ_DATA *obj, bool clone)
{
    if (( clone && obj->light == obj->pIndexData->light)
    ||	(!clone && obj->light != obj->pIndexData->light)
    ||	obj->light == NULL)
	return obj->light; /* shallow copy */

    else
    {
	LIGHT_DATA *light;

	light			= new_light();
	light->duration		= obj->light->duration;
	light->max_duration	= obj->light->max_duration;
	light->fuel		= obj->light->fuel;
	light->intensity	= obj->light->intensity;

	return light;
      }
}
	









	
	
