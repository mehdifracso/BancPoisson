#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
int x=0,y=0;

typedef struct cairoWindow
{
    GtkWidget *window;
    guint sourceid;
    guint deplacement;
    guint vitesse;
    cairo_surface_t *image;
} CairoWindow;

void deplacement_Aleatoire(int *x,int *y,int xMAX,int yMAX,CairoWindow *cairoWindow)
{
    int orientation = rand() % 8;
    printf("\n[orientation] %d",orientation);
    printf("\nNow [x] %d [y] %d\n",*x,*y);

    switch(orientation)
    {
    case 0:
        (*x)-=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("left.png");
        puts("Left");
        break;
    case 1:
        (*x)-=cairoWindow->deplacement;
        (*y)-=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("up_left.png");
        puts("Up - left");
        break;
    case 2:
        (*y)-=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("up.png");
        puts("Up");
        break;
    case 3:
        (*x)+=cairoWindow->deplacement;
        (*y)-=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("up_right.png");
        puts("Up - Right");
        break;
    case 4:
        (*x)+=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("right.png");
        puts("Right");
        break;
    case 5:
        (*x)+=cairoWindow->deplacement;
        (*y)+=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("down_right.png");
        puts("Down - Right");
        break;
    case 6:
        (*y)+=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("down.png");
        puts("Down");
        break;
    case 7:
        (*x)-=cairoWindow->deplacement;
        (*y)+=cairoWindow->deplacement;
        cairoWindow->image = cairo_image_surface_create_from_png("down_left.png");
        puts("Down - Left");
        break;
    }

    printf("\nAfter [x] %d [y] %d",*x,*y);

    if( *y<0 && 0<*x && *x<xMAX)
    {
        *y = yMAX;
    }
    else if( *y>yMAX && 0<*x && *x<xMAX)
    {
        *y = 0;
    }
    else if( *x < 0 && 0<*y && *y<yMAX)
    {
        *x = xMAX;
    }
    else if( *x > xMAX && 0<*y && *y<yMAX)
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

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,CairoWindow *cairoWindow)
{
    int width,heigth;
    gtk_window_get_size(GTK_WINDOW(cairoWindow->window),&width,&heigth);
   srand(time(NULL));

   /*   cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 0.2);

    int i;
    for (i = 0; i <= width; i+=cairoWindow->deplacement )
    {
        /// ligne vertical
        cairo_move_to(cr,i,0);
     //   cairo_line_to(cr,i,heigth);
    }
    for (i = 0; i <= heigth; i+=cairoWindow->deplacement )
    {
        /// ligne horizontale
        cairo_move_to(cr,0,i);
        cairo_line_to(cr,width,i);

    }
    cairo_stroke(cr);
    // Initialization, should only be called once.

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        deplacement_Aleatoire(&x,&y,(width/20)*20-cairoWindow->deplacement,(heigth/20)*20-cairoWindow->deplacement,cairoWindow);
        cairo_rectangle(cr,x,y, cairoWindow->deplacement, cairoWindow->deplacement);
        cairo_fill(cr);
    */

    deplacement_Aleatoire(&x,&y,(width/20)*20-cairoWindow->deplacement,(heigth/20)*20-cairoWindow->deplacement,cairoWindow);
    cairo_set_source_surface(cr, cairoWindow->image, x, y);

    cairo_paint(cr);

    return FALSE;
}

static gboolean time_handler(GtkWidget *widget)//,CairoWindow *cW)
{
//    cW.count += 1;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event,CairoWindow *cairoWindow)
{
    if (event->button == 1)///lift-click
    {


    }

    if (event->button == 3) ///right-click
    {

    }

    return TRUE;
}

static void stop_animation(GtkWidget *button,CairoWindow *cairoWindow)
{
    printf("\nremoving %d",cairoWindow->sourceid);
    g_source_remove(cairoWindow->sourceid);
    cairoWindow->vitesse+=10;
    printf("\nvitesse %d",cairoWindow->vitesse);
}

static void start_animation(GtkWidget *button,CairoWindow *cairoWindow)
{
    g_source_remove(cairoWindow->sourceid);
    cairoWindow->sourceid = g_timeout_add(cairoWindow->vitesse, (GSourceFunc) time_handler, (gpointer) cairoWindow->window);
    printf("\nstarting %d",cairoWindow->sourceid);
}

static void change_vitesse(GtkRange *range,CairoWindow *cairoWindow)
{

    cairoWindow->vitesse = gtk_range_get_value (range);
    printf("------------>%d",cairoWindow->vitesse);
    g_source_remove(cairoWindow->sourceid);
    cairoWindow->sourceid = g_timeout_add(cairoWindow->vitesse, (GSourceFunc) time_handler, (gpointer) cairoWindow->window);
    printf("\nstarting %d",cairoWindow->sourceid);
}

static void change_deplacement(GtkRange *range,CairoWindow *cairoWindow)
{

    cairoWindow->deplacement = gtk_range_get_value (range);
    printf("------------>%d",cairoWindow->vitesse);
    g_source_remove(cairoWindow->sourceid);
    cairoWindow->sourceid = g_timeout_add(cairoWindow->vitesse, (GSourceFunc) time_handler, (gpointer) cairoWindow->window);
    printf("\nstarting %d",cairoWindow->sourceid);
}

int main(int argc, char *argv[])
{
    CairoWindow cairoWindow;

    GtkWidget *boxH;
    GtkWidget *boxV;

    GtkWidget *buttonStart;
    GtkWidget *buttonStop;

    GtkWidget *scale;
    GtkWidget *speedScale;

    GtkWidget *darea;

    gtk_init(&argc, &argv);

    cairoWindow.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request((cairoWindow.window),400,300);
    gtk_window_set_position(GTK_WINDOW(cairoWindow.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(cairoWindow.window), 400, 300);
    gtk_window_set_title(GTK_WINDOW(cairoWindow.window), "Lines");
    //  gtk_window_set_resizable(GTK_WINDOW(cW.window),FALSE);

    boxV=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    boxH=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

    cairoWindow.vitesse=100;
    cairoWindow.deplacement=20;

    buttonStart = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(buttonStart),"Start");

    buttonStop = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(buttonStop),"Stop");

    speedScale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,10,1000,10);
    gtk_scale_set_digits(GTK_SCALE(speedScale),0);
    gtk_scale_set_draw_value(GTK_SCALE(speedScale),1);
    gtk_scale_set_has_origin(GTK_SCALE(speedScale),0);
    gtk_scale_set_value_pos(GTK_SCALE(speedScale),0);

    scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,10,1000,10);
    gtk_scale_set_digits(GTK_SCALE(scale),0);
    gtk_scale_set_draw_value(GTK_SCALE(scale),1);
    gtk_scale_set_has_origin(GTK_SCALE(scale),0);
    gtk_scale_set_value_pos(GTK_SCALE(scale),0);

    darea = gtk_drawing_area_new();

    gtk_widget_add_events(cairoWindow.window, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), &cairoWindow);
    g_signal_connect(cairoWindow.window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(cairoWindow.window, "button-press-event",
                     G_CALLBACK(clicked), &cairoWindow);

    g_signal_connect (GTK_BUTTON (buttonStart),
                      "clicked",
                      G_CALLBACK (start_animation),
                      &cairoWindow);

    g_signal_connect (GTK_BUTTON (buttonStop),
                      "clicked",
                      G_CALLBACK (stop_animation),
                      &cairoWindow);

    g_signal_connect(G_OBJECT(speedScale), "value-changed",G_CALLBACK (change_vitesse),&cairoWindow);

    g_signal_connect(G_OBJECT(scale), "value-changed",G_CALLBACK (change_deplacement),&cairoWindow);

    gtk_box_pack_start(GTK_BOX(boxH),buttonStart,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),buttonStop,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),speedScale,TRUE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxH),scale,TRUE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxV),boxH,FALSE,TRUE,FALSE);
    gtk_box_pack_start(GTK_BOX(boxV),darea,TRUE,TRUE,TRUE);

    gtk_container_add(GTK_CONTAINER(cairoWindow.window), boxV);

    gtk_widget_show_all(cairoWindow.window);
    gtk_main();

    return 0;
}
