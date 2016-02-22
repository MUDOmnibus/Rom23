/* files used in db.c */

/* vals from db.c */
extern bool fBootDb;
extern int		newmobs;
extern int		newobjs;
extern MOB_INDEX_DATA 	* mob_index_hash          [MAX_KEY_HASH];
extern OBJ_INDEX_DATA 	* obj_index_hash          [MAX_KEY_HASH];
extern int		top_mob_index;
extern int		top_obj_index;
extern int  		top_affect;
extern int		top_ed; 
extern int		top_weapon;
extern int		top_armor;
extern int		top_cont;
extern int		top_food;
extern int		top_spells;
extern int		top_use;
extern int		top_light;

/* free lists */
WEAPON_DATA		*weapon_free;
ARMOR_DATA		*armor_free;
CONT_DATA		*cont_free;
FOOD_DATA		*food_free;
SPELL_DATA		*spell_free;
USE_DATA		*use_free;
LIGHT_DATA		*light_free;


/* from db2.c */
extern int	social_count;
