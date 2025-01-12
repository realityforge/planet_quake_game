/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1997-1999, 2000-2001 Tim Janik and Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * gvalue.h: generic GValue functions
 */
#if !defined (__GLIB_GOBJECT_H_INSIDE__) && !defined (GOBJECT_COMPILATION)
#error "Only <glib-object.h> can be included directly."
#endif

#ifndef __G_VALUE_H__
#define __G_VALUE_H__

#include	<gobject/gtype.h>

G_BEGIN_DECLS

/* --- type macros --- */
/**
 * G_TYPE_IS_VALUE:
 * @type: A #GType value.
 * 
 * Checks whether the passed in type ID can be used for g_value_init().
 * That is, this macro checks whether this type provides an implementation
 * of the #GTypeValueTable functions required for a type to create a #GValue of.
 * 
 * Returns: Whether @type is suitable as a #GValue type.
 */
#define	G_TYPE_IS_VALUE(type)		(g_type_check_is_value_type (type))
/**
 * G_IS_VALUE:
 * @value: A #GValue structure.
 * 
 * Checks if @value is a valid and initialized #GValue structure.
 *
 * Returns: %TRUE on success.
 */
#define	G_IS_VALUE(value)		(G_TYPE_CHECK_VALUE (value))
/**
 * G_VALUE_TYPE:
 * @value: A #GValue structure.
 * 
 * Get the type identifier of @value.
 *
 * Returns: the #GType.
 */
#define	G_VALUE_TYPE(value)		(((GValue*) (value))->g_type)
/**
 * G_VALUE_TYPE_NAME:
 * @value: A #GValue structure.
 *
 * Gets the the type name of @value. 
 *
 * Returns: the type name.
 */
#define	G_VALUE_TYPE_NAME(value)	(g_type_name (G_VALUE_TYPE (value)))
/**
 * G_VALUE_HOLDS:
 * @value: A #GValue structure.
 * @type: A #GType value.
 * 
 * Checks if @value holds (or contains) a value of @type.
 * This macro will also check for @value != %NULL and issue a
 * warning if the check fails.
 *
 * Returns: %TRUE if @value holds the @type.
 */
#define G_VALUE_HOLDS(value,type)	(G_TYPE_CHECK_VALUE_TYPE ((value), (type)))


/* --- typedefs & structures --- */
/**
 * GValueTransform:
 * @src_value: Source value.
 * @dest_value: Target value.
 * 
 * The type of value transformation functions which can be registered with
 * g_value_register_transform_func().
 */
typedef void (*GValueTransform) (const GValue *src_value,
				 GValue       *dest_value);
/**
 * GValue:
 * 
 * An opaque structure used to hold different types of values.
 * The data within the structure has protected scope: it is accessible only
 * to functions within a #GTypeValueTable structure, or implementations of
 * the g_value_*() API. That is, code portions which implement new fundamental
 * types.
 * #GValue users can not make any assumptions about how data is stored
 * within the 2 element @data union, and the @g_type member should
 * only be accessed through the G_VALUE_TYPE() macro.
 */
struct _GValue
{
  /*< private >*/
  GType		g_type;

  /* public for GTypeValueTable methods */
  union {
    gint	v_int;
    guint	v_uint;
    glong	v_long;
    gulong	v_ulong;
    gint64      v_int64;
    guint64     v_uint64;
    gfloat	v_float;
    gdouble	v_double;
    gpointer	v_pointer;
  } data[2];
};


/* --- prototypes --- */
GValue*         g_value_init	   	(GValue       *value,
					 GType         g_type);
void            g_value_copy    	(const GValue *src_value,
					 GValue       *dest_value);
GValue*         g_value_reset   	(GValue       *value);
void            g_value_unset   	(GValue       *value);
void		g_value_set_instance	(GValue	      *value,
					 gpointer      instance);


/* --- private --- */
gboolean	g_value_fits_pointer	(const GValue *value);
gpointer	g_value_peek_pointer	(const GValue *value);


/* --- implementation details --- */
gboolean g_value_type_compatible	(GType		 src_type,
					 GType		 dest_type);
gboolean g_value_type_transformable	(GType           src_type,
					 GType           dest_type);
gboolean g_value_transform		(const GValue   *src_value,
					 GValue         *dest_value);
void	g_value_register_transform_func	(GType		 src_type,
					 GType		 dest_type,
					 GValueTransform transform_func);
#define G_VALUE_NOCOPY_CONTENTS		(1 << 27)


G_END_DECLS

#endif /* __G_VALUE_H__ */
