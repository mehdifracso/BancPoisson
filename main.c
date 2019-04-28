#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

#define DEP 20
int x=0,y=0;
void deplacement_Aleatoire(int *x,int *y,int xMAX,int yMAX)
{
    int orientation=random_number(0,7);

    switch(orientation)
    {
    case 0:
        (*x)-=DEP;
        break;
    case 1:
        (*x)-=DEP;
        (*y)-=DEP;
        break;
    case 2:
        (*y)-=DEP;
        break;
    case 3:
        (*x)+=DEP;
        (*y)-=DEP;
        break;
    case 4:
        (*x)+=DEP;
        break;
    case 5:
        (*x)+=DEP;
        (*y)+=DEP;
        break;
    case 6:
        (*y)+=DEP;
        break;
    case 7:
        (*x)-=DEP;
        (*y)+=DEP;
        break;
    }

        if( *y<0 && 0<*x<xMAX)
        {
            *y = yMAX;
        }
        else if( *y>yMAX && 0<*x<xMAX)
        {
            *y = 0;
        }
        else if( *x < 0 && 0<*y<yMAX)
        {
            *x = xMAX;
        }
        else if( *x > xMAX && 0<*y<yMAX)
        {
            *x = 0;
        }
        else if( *y < 0 && *x > xMAX)
        {
            *y = yMAX;
            *x = 0;
        }
        else if( *y > yMAX && *x < 0)
        {
            *y = 0;
            *x = xMAX;
        }
        else if( *x < 0 && *y< 0)
        {
            *x = xMAX;
            *y = yMAX;
        }
        else if( *x > xMAX && *y > yMAX)
        {
            *x = 0;
            *y = 0;
        }

}

int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    }
    else
    {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,gpointer window)
{

    int width,heigth;
    gtk_window_get_size(GTK_WINDOW(window),&width,&heigth);
    printf("\n[x] %d [y] %d",x,y);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 0.2);

    int i;
    for (i = 0; i <= width; i+=DEP )
    {
        /// ligne vertical
        cairo_move_to(cr,i,0);
        cairo_line_to(cr,i,heigth);
    }
    for (i = 0; i <= heigth; i+=DEP )
    {
        /// ligne horizontale
        cairo_move_to(cr,0,i);
        cairo_line_to(cr,width,i);

    }
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    deplacement_Aleatoire(&x,&y,(width/20)*20,(heigth/20)*20);

    cairo_rectangle(cr, x, y, DEP, DEP);

    cairo_fill(cr);
    return FALSE;
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *darea;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

   g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), window);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_title(GTK_WINDOW(window), "Lines");

///    on_draw_event(darea),)

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
