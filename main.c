#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
int x=0,y=0;

typedef struct cW
{
    GtkWidget *window;
    guint sourceid;
    guint deplacement;
    guint vitesse;
    gushort count;
} CairoWindow;

void deplacement_Aleatoire(int *x,int *y,int xMAX,int yMAX,CairoWindow *cW)
{
    int orientation=random_number(0,7);
    printf("\n[orientation] %d",orientation);
    printf("\nNow [x] %d [y] %d\n",*x,*y);

    switch(orientation)
    {
    case 0:
        (*x)-=cW->deplacement;
        puts("Left");
        break;
    case 1:
        (*x)-=cW->deplacement;
        (*y)-=cW->deplacement;
        puts("Up - left");
        break;
    case 2:
        (*y)-=cW->deplacement;
        puts("Up");
        break;
    case 3:
        (*x)+=cW->deplacement;
        (*y)-=cW->deplacement;
        puts("Up - Right");
        break;
    case 4:
        (*x)+=cW->deplacement;
        puts("Right");
        break;
    case 5:
        (*x)+=cW->deplacement;
        (*y)+=cW->deplacement;
        puts("Down - Right");
        break;
    case 6:
        (*y)+=cW->deplacement;
        puts("Down");
        break;
    case 7:
        (*x)-=cW->deplacement;
        (*y)+=cW->deplacement;
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
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,CairoWindow *cW)
{
    int width,heigth;
    gtk_window_get_size(GTK_WINDOW(cW->window),&width,&heigth);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 0.2);

    int i;
    for (i = 0; i <= width; i+=cW->deplacement )
    {
        /// ligne vertical
        cairo_move_to(cr,i,0);
        cairo_line_to(cr,i,heigth);
    }
    for (i = 0; i <= heigth; i+=cW->deplacement )
    {
        /// ligne horizontale
        cairo_move_to(cr,0,i);
        cairo_line_to(cr,width,i);

    }
    cairo_stroke(cr);

    deplacement_Aleatoire(&x,&y,(width/20)*20-cW->deplacement,(heigth/20)*20-cW->deplacement,cW);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    cairo_rectangle(cr,x,y, cW->deplacement, cW->deplacement);

    cairo_fill(cr);

    return FALSE;
}

static gboolean time_handler(GtkWidget *widget)//,CairoWindow *cW)
{
//    cW.count += 1;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event,CairoWindow *cW)
{
    if (event->button == 1)///lift-click
    {


    }

    if (event->button == 3) ///right-click
    {

    }

    return TRUE;
}

static void stop_animation(GtkWidget *button,CairoWindow *cW)
{
    printf("\nremoving %d",cW->sourceid);
    g_source_remove(cW->sourceid);
    cW->vitesse+=10;
    printf("\nvitesse %d",cW->vitesse);
}

static void start_animation(GtkWidget *button,CairoWindow *cW)
{
    g_source_remove(cW->sourceid);
    cW->sourceid = g_timeout_add(cW->vitesse, (GSourceFunc) time_handler, (gpointer) cW->window);
    printf("\nstarting %d",cW->sourceid);
}

static void change_vitesse(GtkRange *range,CairoWindow *cW)
{

    cW->vitesse = gtk_range_get_value (range);
    printf("------------>%d",cW->vitesse);
    g_source_remove(cW->sourceid);
    cW->sourceid = g_timeout_add(cW->vitesse, (GSourceFunc) time_handler, (gpointer) cW->window);
    printf("\nstarting %d",cW->sourceid);
}

static void change_deplacement(GtkRange *range,CairoWindow *cW)
{

    cW->deplacement = gtk_range_get_value (range);
    printf("------------>%d",cW->vitesse);
    g_source_remove(cW->sourceid);
    cW->sourceid = g_timeout_add(cW->vitesse, (GSourceFunc) time_handler, (gpointer) cW->window);
    printf("\nstarting %d",cW->sourceid);
}

int main(int argc, char *argv[])
{
    CairoWindow cW;

    GtkWidget *boxH;
    GtkWidget *boxV;

    GtkWidget *buttonStart;
    GtkWidget *buttonStop;

    GtkWidget *scale;
    GtkWidget *speedScale;

    GtkWidget *darea;

    gtk_init(&argc, &argv);

    cW.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(GTK_WINDOW(cW.window),400,300);
    gtk_window_set_position(GTK_WINDOW(cW.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(cW.window), 400, 300);
    gtk_window_set_title(GTK_WINDOW(cW.window), "Lines");
    //  gtk_window_set_resizable(GTK_WINDOW(cW.window),FALSE);

    boxV=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    boxH=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

    cW.vitesse=100;
    cW.deplacement=20;

    buttonStart = gtk_button_new();
    gtk_button_set_label(buttonStart,"Start");

    buttonStop = gtk_button_new();
    gtk_button_set_label(buttonStop,"Stop");

    speedScale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1000,10);
    gtk_scale_set_digits(speedScale,0);
    gtk_scale_set_draw_value(speedScale,1);
    gtk_scale_set_has_origin(speedScale,0);
    gtk_scale_set_value_pos(speedScale,0);

    scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1000,10);
    gtk_scale_set_digits(scale,0);
    gtk_scale_set_draw_value(scale,1);
    gtk_scale_set_has_origin(scale,0);
    gtk_scale_set_value_pos(scale,0);

    darea = gtk_drawing_area_new();

    gtk_widget_add_events(cW.window, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), &cW);
    g_signal_connect(cW.window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(cW.window, "button-press-event",
                     G_CALLBACK(clicked), &cW);

    g_signal_connect (GTK_BUTTON (buttonStart),
                      "clicked",
                      G_CALLBACK (start_animation),
                      &cW);

    g_signal_connect (GTK_BUTTON (buttonStop),
                      "clicked",
                      G_CALLBACK (stop_animation),
                      &cW);

    g_signal_connect(G_OBJECT(speedScale), "value-changed",G_CALLBACK (change_vitesse),&cW);

    g_signal_connect(G_OBJECT(scale), "value-changed",G_CALLBACK (change_deplacement),&cW);

    gtk_box_pack_start(GTK_BOX(boxH),buttonStart,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),buttonStop,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),speedScale,TRUE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),scale,TRUE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxV),boxH,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxV),darea,TRUE,TRUE,TRUE);

    gtk_container_add(GTK_CONTAINER(cW.window), boxV);

    gtk_widget_show_all(cW.window);
    gtk_main();

    return 0;
}
