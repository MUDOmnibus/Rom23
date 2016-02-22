/* prototypes for all the lovely new object formats */

struct weapon_data
{
    WEAPON_DATA	*next;		/* for free list */
    sh_int	type;		/* weapon type */
    sh_int	message;	/* damage message */
    sh_int	blade;		/* what the blade is made of */
    sh_int	flags;		/* weapon flags */
    sh_int	hit;		/* hit bounus */
    sh_int	damage[3];	/* damage dice */
    char	*slaying;	/* name of race it will slay */
};


struct armor_data
{
    ARMOR_DATA 	*next;		/* for free list */
    sh_int	ac[4];		/* ac values */
    sh_int	bulk;		/* how heavy is YOUR armor? */
    sh_int	bonus;		/* magical bonus */
};

struct cont_data  	/* containers, drink containers, fountains */
{
    CONT_DATA 	*next;		/* for free list */
    sh_int	closed;		/* open, closed, etc */
    sh_int	key;		/* for containers only */
    sh_int	volume;		/* liquid content OR max weight carried */
    sh_int	limit;		/* liquid max OR max weight for one item */
    sh_int	liquid;		/* for drinks only */
    bool 	poison;		/* for drinks only */
};

struct food_data
{
    FOOD_DATA	*next;		/* for free list */
    sh_int	full;		/* total food value */
    sh_int	bite;		/* how many food units per eat */
    sh_int	taste;		/* how tasty it is */
    bool	poison;		/* poisoned or safe */
};


struct spell_data	/* potions, pills, scrolls */
{
    SPELL_DATA	*next;		/* for free list */
    sh_int	level;		/* level of spells */
    sh_int	class;		/* 0 is mage, 1 is cleric */
    sh_int	delay;		/* delay of item */
    sh_int	spell[4];	/* spells on item */
};


struct use_data		/* wands, staves */
{
    USE_DATA	*next;		/* for free list */
    sh_int	level;		/* level of spell */
    sh_int	class;		/* 0 is mage, 1 is cleric */
    sh_int	delay;		/* delay of item */
    sh_int	charges;	/* number of charges */
    sh_int	max;		/* max charges */
    sh_int 	spell;		/* spell of item */
};


struct light_data
{
    LIGHT_DATA	*next;		/* for free list */
    sh_int	duration;	/* duration of light */
    sh_int	max_duration;	/* some can be reused */
    sh_int	fuel;		/* liquid # used for fueling */
    sh_int	intensity;	/* how bright the light is */
};


/* prototype for an object */
struct obj_index_data	
{
    OBJ_INDEX_DATA *next;
    EXTRA_DESCR_DATA *extra_descr;
    AFFECT_DATA *affected;

    /* item type data */
    int 	type;

    WEAPON_DATA	*weapon;
    ARMOR_DATA 	*armor;
    CONT_DATA	*cont;
    FOOD_DATA	*food;
    SPELL_DATA	*spells;
    USE_DATA	*use;
    LIGHT_DATA 	*light;

    /* other data */
    char 	*name;
    char	*short_descr;
    char	*description;
    sh_int	vnum;
    sh_int	reset_num;
    sh_int	material;
    int		extra_flags;
    sh_int	wear_flags;
    sh_int	level;
    sh_int	condition;
    sh_int	count;
    sh_int	weight;
    int		cost;
};

/* one object */
struct obj_data
{
    OBJ_DATA 		*next;
    OBJ_DATA		*next_content;
    OBJ_DATA		*contains;
    OBJ_DATA 		*in_obj;
    CHAR_DATA		*carried_by;
    EXTRA_DESCR_DATA	*extra_descr;
    AFFECT_DATA		*affected;
    OBJ_INDEX_DATA	*pIndexData;
    ROOM_INDEX_DATA 	* in_room;

    /* item type data */
    int         type;
 
    WEAPON_DATA *weapon;
    ARMOR_DATA  *armor;
    CONT_DATA   *cont;
    FOOD_DATA   *food;
    SPELL_DATA  *spells;
    USE_DATA    *use; 
    LIGHT_DATA	*light;

    /* other data */
    bool	enchanted;
    char	*owner;
    char	*name;
    char	*short_descr;
    char	*description;
    int		extra_flags;
    sh_int	wear_flags;
    sh_int	wear_loc;
    sh_int	weight;
    int		cost;
    sh_int	level;
    sh_int	condition;
    sh_int	material;
    sh_int	timer;
};



 
/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT                    A
#define ITEM_SCROLL                   B
#define ITEM_WAND                     C
#define ITEM_STAFF                    D
#define ITEM_WEAPON                   E
#define ITEM_TREASURE                 F
#define ITEM_ARMOR                    G
#define ITEM_POTION                   H
#define ITEM_CLOTHING                 I
#define ITEM_FURNITURE                J
#define ITEM_TRASH                    K
#define ITEM_CONTAINER                L
#define ITEM_DRINK_CON                M
#define ITEM_KEY                      N
#define ITEM_FOOD                     O
#define ITEM_MONEY                    P 
#define ITEM_BOAT                     Q
#define ITEM_CORPSE_NPC               R
#define ITEM_CORPSE_PC                S
#define ITEM_FOUNTAIN                 T
#define ITEM_PILL                     U
#define ITEM_MAP                      V
 
 
/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW               (A)
#define ITEM_HUM                (B)
#define ITEM_DARK               (C)
#define ITEM_LOCK               (D)
#define ITEM_EVIL               (E)
#define ITEM_INVIS              (F)
#define ITEM_MAGIC              (G)
#define ITEM_NODROP             (H)
#define ITEM_BLESS              (I)
#define ITEM_ANTI_GOOD          (J)
#define ITEM_ANTI_EVIL          (K)
#define ITEM_ANTI_NEUTRAL       (L)
#define ITEM_NOREMOVE           (M)
#define ITEM_INVENTORY          (N)
#define ITEM_NOPURGE            (O)
#define ITEM_ROT_DEATH          (P)
#define ITEM_VIS_DEATH          (Q)
 
 
/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE               (A)
#define ITEM_WEAR_FINGER        (B)
#define ITEM_WEAR_NECK          (C)
#define ITEM_WEAR_BODY          (D)
#define ITEM_WEAR_HEAD          (E)
#define ITEM_WEAR_LEGS          (F)
#define ITEM_WEAR_FEET          (G)
#define ITEM_WEAR_HANDS         (H)
#define ITEM_WEAR_ARMS          (I)
#define ITEM_WEAR_SHIELD        (J)
#define ITEM_WEAR_ABOUT         (K)
#define ITEM_WEAR_WAIST         (L)
#define ITEM_WEAR_WRIST         (M)
#define ITEM_WIELD              (N)
#define ITEM_HOLD               (O)
#define ITEM_TWO_HANDS          (P)
 
/* weapon class */
#define WEAPON_EXOTIC           0
#define WEAPON_SWORD            1
#define WEAPON_DAGGER           2
#define WEAPON_SPEAR            3
#define WEAPON_MACE             4
#define WEAPON_AXE              5
#define WEAPON_FLAIL            6
#define WEAPON_WHIP             7
#define WEAPON_POLEARM          8
 
/* weapon types */
#define WEAPON_FLAMING          (A)
#define WEAPON_FROST            (B)
#define WEAPON_VAMPIRIC         (C)
#define WEAPON_SHARP            (D)
#define WEAPON_VORPAL           (E)
#define WEAPON_TWO_HANDS        (F)
 
 
 
/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE                1
#define CONT_PICKPROOF                2
#define CONT_CLOSED                   4
#define CONT_LOCKED                   8

/* various object procedures */

WEAPON_DATA *	new_weapon	args( (void) );
ARMOR_DATA  * 	new_armor	args( (void) );
CONT_DATA   * 	new_cont	args( (void) );
FOOD_DATA   * 	new_food	args( (void) );
SPELL_DATA  *	new_spells	args( (void) );
USE_DATA    *	new_use		args( (void) );
LIGHT_DATA  *	new_light	args( (void) );

void		free_weapon	args( (OBJ_DATA *obj) );
void		free_armor	args( (OBJ_DATA *obj) );
void		free_cont	args( (OBJ_DATA *obj) );
void		free_food	args( (OBJ_DATA *obj) );
void		free_spells	args( (OBJ_DATA *obj) );
void		free_use	args( (OBJ_DATA *obj) );
void		free_light	args( (OBJ_DATA *obj) );

WEAPON_DATA *	copy_weapon	args( (const OBJ_DATA *obj, bool clone) );
ARMOR_DATA  *	copy_armor	args( (const OBJ_DATA *obj, bool clone) );
CONT_DATA   *	copy_cont	args( (const OBJ_DATA *obj, bool clone) );
FOOD_DATA   *	copy_food	args( (const OBJ_DATA *obj, bool clone) );
SPELL_DATA  *	copy_spells	args( (const OBJ_DATA *obj, bool clone) );
USE_DATA    *	copy_use	args( (const OBJ_DATA *obj, bool clone) );
LIGHT_DATA  *	copy_light	args( (const OBJ_DATA *obj, bool clone) );

