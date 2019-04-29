#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#define DEP 10
int x=0,y=0,vitesse=10;
struct
{
    gushort count;
} glob;

typedef struct wS
{
    guint sourceid;
    GtkWidget *window;
}WindowsSource;

void deplacement_Aleatoire(int *x,int *y,int xMAX,int yMAX)
{
    int orientation=random_number(0,7);
    printf("\n[orientation] %d",orientation);
    printf("\nNow [x] %d [y] %d\n",*x,*y);

    switch(orientation)
    {
    case 0:
        (*x)-=DEP;
        puts("Left");
        break;
    case 1:
        (*x)-=DEP;
        (*y)-=DEP;
        puts("Up - left");
        break;
    case 2:
        (*y)-=DEP;
        puts("Up");
        break;
    case 3:
        (*x)+=DEP;
        (*y)-=DEP;
        puts("Up - Right");
        break;
    case 4:
        (*x)+=DEP;
        puts("Right");
        break;
    case 5:
        (*x)+=DEP;
        (*y)+=DEP;
        puts("Down - Right");
        break;
    case 6:
        (*y)+=DEP;
        puts("Down");
        break;
    case 7:
        (*x)-=DEP;
        (*y)+=DEP;
        puts("Down - Left");
        break;
    }

    printf("\nAfter [x] %d [y] %d",*x,*y);

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

    deplacement_Aleatoire(&x,&y,(width/20)*20-DEP,(heigth/20)*20-DEP);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    cairo_rectangle(cr,x,y, DEP, DEP);

    cairo_fill(cr);

    return FALSE;
}
static gboolean time_handler(GtkWidget *widget)
{
    glob.count += 1;
    gtk_widget_queue_draw(widget);
    return TRUE;
}
static gboolean clicked(GtkWidget *widget, GdkEventButton *event,guint * source_id)
{

    if (event->button == 1)///lift-click
    {


    }

    if (event->button == 3) ///right-click
    {

    }

    return TRUE;
}

static void stop_animation(GtkWidget *button,WindowsSource *wS)
{
        printf("\nremoving %d",wS->sourceid);
        g_source_remove(wS->sourceid);
        vitesse+=10;
        printf("\nvitesse %d",vitesse);
}

static void start_animation(GtkWidget *button,WindowsSource *wS)
{
        g_source_remove(wS->sourceid);
        wS->sourceid = g_timeout_add(vitesse, (GSourceFunc) time_handler, (gpointer) wS->window);
        printf("\nstarting %d",wS->sourceid);
}

static void change_vitesse(GtkRange *range,WindowsSource *wS)
{

        vitesse = gtk_range_get_value (range);
        printf("------------>%d",vitesse);
        g_source_remove(wS->sourceid);
        wS->sourceid = g_timeout_add(vitesse, (GSourceFunc) time_handler, (gpointer) wS->window);
        printf("\nstarting %d",wS->sourceid);
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *boxH;
    GtkWidget *buttonStart;
    GtkWidget *buttonStop;

    GtkWidget *boxV;
    GtkWidget *darea;
    guint source_id;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(GTK_WINDOW(window),400,300);
  //  gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

    boxV=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    boxH=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

    WindowsSource wS;
    wS.window=window;

    buttonStart = gtk_button_new();
    gtk_button_set_label(buttonStart,"Start");

    buttonStop = gtk_button_new();
    gtk_button_set_label(buttonStop,"Stop");

    g_signal_connect (GTK_BUTTON (buttonStart),
                      "clicked",
                      G_CALLBACK (start_animation),
                      &wS);

    g_signal_connect (GTK_BUTTON (buttonStop),
                      "clicked",
                      G_CALLBACK (stop_animation),
                      &wS);

        GtkWidget *scale;
    scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1000,10);
    gtk_scale_set_digits(scale,0);
    gtk_scale_set_draw_value(scale,1);
    gtk_scale_set_has_origin(scale,0);
    gtk_scale_set_value_pos(scale,0);

    g_signal_connect(G_OBJECT(scale), "value-changed",G_CALLBACK (change_vitesse),&wS);

    darea = gtk_drawing_area_new();

    gtk_box_pack_start(GTK_BOX(boxH),buttonStart,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),buttonStop,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),scale,TRUE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxV),boxH,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxV),darea,TRUE,TRUE,TRUE);

    gtk_container_add(GTK_CONTAINER(window), boxV);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), window);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "button-press-event",
                     G_CALLBACK(clicked), &source_id);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_title(GTK_WINDOW(window), "Lines");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
