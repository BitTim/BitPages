#include "../common/lib/imageGenerator.hh"

#include <filesystem>
namespace fs = std::filesystem;

#include "../common/lib/objects.hh"
#include "lib/globals.hh"
#include "lib/gui.hh"

SDL_Surface* generateSurface(int slide)
{
    //Draw Background
    SDL_Surface* surface = SDL_CreateRGBSurface(0, Global::_WIDTH, Global::_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Surface* image = nullptr;

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
    if (Global::_BACKGROUND != "none" && Global::_BACKGROUND != "default")
    {
        if(fs::path(Global::_BACKGROUND).is_relative())
        {
            std::string tmp;
            tmp = Global::_INPATH + Global::_BACKGROUND;
            Global::_BACKGROUND = tmp;
        }

        image = IMG_Load(Global::_BACKGROUND.c_str());
        if (image == NULL) gprintf("[ERROR]: Rendering Background: %s\n", SDL_GetError());
        else SDL_BlitScaled(image, NULL, surface, NULL);
        SDL_FreeSurface(image);
    }

    SDL_Surface* text = nullptr;
    SDL_Rect dst;

    //Draw Content
    if (Global::_PRESENT->slides[slide].titleSlide)
    {
        //Title
        text = TTF_RenderUTF8_Blended_Wrapped(Global::_FONT["title"], Global::_PRESENT->slides[slide].title.c_str(), *Global::_TEXTCOLOR, Global::_WIDTH - 2 * Global::_BORDERS);
        if (text == NULL) gprintf("[ERROR]: Rendering Title: %s\n", TTF_GetError());
        else
        {
            dst = { Global::_WIDTH / 2 - text->w / 2, Global::_HEIGHT / 2 - text->h, text->w, text->h };
            SDL_BlitScaled(text, NULL, surface, &dst);
            SDL_FreeSurface(text);
        }

        //Subtitle
        if (Global::_PRESENT->slides[slide].subtitle.length() > 0)
        {
            text = TTF_RenderUTF8_Blended_Wrapped(Global::_FONT["subtitle"], Global::_PRESENT->slides[slide].subtitle.c_str(), *Global::_TEXTCOLOR, Global::_WIDTH - 2 * Global::_BORDERS);
            if (text == NULL) gprintf("[ERROR]: Rendering Subtitle: %s\n", TTF_GetError());
            else
            {
                dst = { Global::_WIDTH / 2 - text->w / 2, Global::_HEIGHT / 2 + text->h, text->w, text->h };
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }
        }

        //Render "Created with BitPresent"
        if (slide == Global::_PRESENT->slides.size() - 1)
        {
            text = TTF_RenderUTF8_Blended(Global::_FONT["footer"], "Created with BitPresent", *Global::_TEXTCOLOR);
            if (text == NULL) gprintf("[ERROR]: Rendering Footer: %s\n", TTF_GetError());
            else
            {
                dst = { Global::_WIDTH - Global::_BORDERS - text->w, Global::_HEIGHT - Global::_BORDERS - text->h, text->w, text->h };
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }
        }
    }
    else
    {
        //Title
        text = TTF_RenderUTF8_Blended_Wrapped(Global::_FONT["title"], Global::_PRESENT->slides[slide].title.c_str(), *Global::_TEXTCOLOR, Global::_WIDTH - 2 * Global::_BORDERS);
        if (text == NULL) gprintf("[ERROR]: Rendering Title: %s\n", TTF_GetError());
        else
        {
            dst = { Global::_BORDERS, Global::_BORDERS, text->w, text->h };
            SDL_BlitScaled(text, NULL, surface, &dst);
            SDL_FreeSurface(text);
        }

        //Subtitle
        if (Global::_PRESENT->slides[slide].subtitle.length() > 0)
        {
            text = TTF_RenderUTF8_Blended_Wrapped(Global::_FONT["subtitle"], Global::_PRESENT->slides[slide].subtitle.c_str(), *Global::_TEXTCOLOR, Global::_WIDTH - 2 * Global::_BORDERS);
            if (text == NULL) gprintf("[ERROR]: Rendering Subtitle: %s\n", TTF_GetError());
            else
            {
                dst = { Global::_BORDERS, dst.y + dst.h, text->w, text->h };
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }
        }

        //Image
        int contentHeight = dst.y + dst.h * 2;
        int imgPosX = Global::_WIDTH;

        if (Global::_PRESENT->slides[slide].image != "none")
        {
            if(fs::path(Global::_PRESENT->slides[slide].image).is_relative())
            {
                std::string tmp;
                tmp = Global::_INPATH + Global::_PRESENT->slides[slide].image;
                Global::_PRESENT->slides[slide].image = tmp;
            }

            image = IMG_Load(Global::_PRESENT->slides[slide].image.c_str());
            if (image == NULL) gprintf("[ERROR]: Rendering Image: %s\n", SDL_GetError());
            else
            {
                dst.h = Global::_HEIGHT - contentHeight - Global::_BORDERS;
                dst.w = int(float(float(dst.h) / float(image->h) * float(image->w)));
                dst.x = Global::_WIDTH - Global::_BORDERS - dst.w;
                dst.y = Global::_HEIGHT - Global::_BORDERS - dst.h;

                if (dst.w > Global::_WIDTH / 2)
                {
                    SDL_Rect tmp = dst;

                    dst.w = Global::_WIDTH / 2;
                    dst.h = int(float(float(dst.w) / float(tmp.w) * float(tmp.h)));
                    dst.x = Global::_WIDTH - Global::_BORDERS - dst.w;
                    dst.y = Global::_HEIGHT - Global::_BORDERS - dst.h;
                }

                imgPosX = dst.x;

                SDL_BlitScaled(image, NULL, surface, &dst);
                SDL_FreeSurface(image);
            }
        }

        //Points
        for (int i = 0; i < Global::_PRESENT->slides[slide].points.size(); i++)
        {
            //Render Dashes
            text = TTF_RenderUTF8_Blended(Global::_FONT["normal"], "- ", *Global::_TEXTCOLOR);
            if (text == NULL) gprintf("[ERROR]: Rendering Prefix: %s\n", TTF_GetError());
            else
            {
                dst = { Global::_BORDERS, contentHeight, text->w, text->h };
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }

            //Render Text
            text = TTF_RenderUTF8_Blended_Wrapped(Global::_FONT["normal"], Global::_PRESENT->slides[slide].points[i].text.c_str(), *Global::_TEXTCOLOR, imgPosX - 2 * Global::_BORDERS - dst.x);
            if (text == NULL) gprintf("[ERROR]: Rendering Points: %s\n", TTF_GetError());
            else
            {
                dst = { Global::_BORDERS + dst.w, contentHeight, text->w, text->h };
                contentHeight += text->h;
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }

            //Render Subpoints
            for(int j = 0; j < Global::_PRESENT->slides[slide].points[i].subPoints.size(); j++)
            {
                //Render Dots
                text = TTF_RenderUTF8_Blended(Global::_FONT["normal"], "> ", *Global::_TEXTCOLOR);
                if (text == NULL) gprintf("[ERROR]: Rendering Prefix: %s\n", TTF_GetError());
                else
                {
                    dst = { Global::_BORDERS + Global::_INDENT, contentHeight, text->w, text->h };
                    SDL_BlitScaled(text, NULL, surface, &dst);
                    SDL_FreeSurface(text);
                }

                //Render Text
                text = TTF_RenderUTF8_Blended_Wrapped(Global::_FONT["normal"], Global::_PRESENT->slides[slide].points[i].subPoints[j].c_str(), *Global::_TEXTCOLOR, imgPosX - 2 * Global::_BORDERS - dst.x);
                if (text == NULL) gprintf("[ERROR]: Rendering Points: %s\n", TTF_GetError());
                else
                {
                    dst = { Global::_BORDERS + Global::_INDENT + dst.w, contentHeight, text->w, text->h };
                    contentHeight += text->h;
                    SDL_BlitScaled(text, NULL, surface, &dst);
                    SDL_FreeSurface(text);
                }
            }
        }

        //Render "Created with BitPresent"
        if (slide == Global::_PRESENT->slides.size() - 1)
        {
            text = TTF_RenderUTF8_Blended(Global::_FONT["footer"], "Created with BitPresent", *Global::_TEXTCOLOR);
            if (text == NULL) gprintf("[ERROR]: Rendering Footer: %s\n", TTF_GetError());
            else
            {
                dst = { Global::_WIDTH - Global::_BORDERS - text->w, Global::_HEIGHT - Global::_BORDERS - text->h, text->w, text->h };
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }
        }
    }

    return surface;
}

void saveImage(SDL_Surface* surface, std::string path)
{
    IMG_SavePNG(surface, path.c_str());
}