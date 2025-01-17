#include "lib/imageGenerator.hh"

#include <filesystem>
namespace fs = std::filesystem;

#include "lib/objects.hh"
#include "lib/globals.hh"
#include "lib/gui.hh"

SDL_Surface* generateSurface(int slide)
{
    //Draw Background
    SDL_Surface* surface = SDL_CreateRGBSurface(0, Global::_WIDTH, Global::_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Surface* image = nullptr;

    if (surface == nullptr)
    {
        Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Failed to create surface for image"));
        return nullptr;
    }

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
    if (Global::_PRESENT->background != "none" && Global::_PRESENT->background != "default")
    {
        image = IMG_Load(Global::_PRESENT->background.c_str());
        if (image == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Failed to load background image: " + std::string(SDL_GetError())));
        else SDL_BlitScaled(image, NULL, surface, NULL);
        SDL_FreeSurface(image);
    }

    SDL_Surface* text = nullptr;
    SDL_Rect dst;

    //Draw Content
    if (Global::_PRESENT->slides[slide].titleSlide)
    {
        //Title
        text = TTF_RenderUTF8_Blended_Wrapped(Global::_PRESENT->font["title"], Global::_PRESENT->slides[slide].title.c_str(), *Global::_PRESENT->textcolor, Global::_WIDTH - 2 * Global::_BORDERS);
        if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering title: " + std::string(TTF_GetError())));
        else
        {
            dst = { Global::_WIDTH / 2 - text->w / 2, Global::_HEIGHT / 2 - text->h, text->w, text->h };
            SDL_BlitScaled(text, NULL, surface, &dst);
            SDL_FreeSurface(text);
        }

        //Subtitle
        if (Global::_PRESENT->slides[slide].subtitle.length() > 0)
        {
            text = TTF_RenderUTF8_Blended_Wrapped(Global::_PRESENT->font["subtitle"], Global::_PRESENT->slides[slide].subtitle.c_str(), *Global::_PRESENT->textcolor, Global::_WIDTH - 2 * Global::_BORDERS);
            if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering subtitle: " + std::string(TTF_GetError())));
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
            text = TTF_RenderUTF8_Blended(Global::_PRESENT->font["footer"], "Created with BitPresent", *Global::_PRESENT->textcolor);
            if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering footer: " + std::string(TTF_GetError())));
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
        text = TTF_RenderUTF8_Blended_Wrapped(Global::_PRESENT->font["title"], Global::_PRESENT->slides[slide].title.c_str(), *Global::_PRESENT->textcolor, Global::_WIDTH - 2 * Global::_BORDERS);
        if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering title: " + std::string(TTF_GetError())));
        else
        {
            dst = { Global::_BORDERS, Global::_BORDERS, text->w, text->h };
            SDL_BlitScaled(text, NULL, surface, &dst);
            SDL_FreeSurface(text);
        }

        //Subtitle
        if (Global::_PRESENT->slides[slide].subtitle.length() > 0)
        {
            text = TTF_RenderUTF8_Blended_Wrapped(Global::_PRESENT->font["subtitle"], Global::_PRESENT->slides[slide].subtitle.c_str(), *Global::_PRESENT->textcolor, Global::_WIDTH - 2 * Global::_BORDERS);
            if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering subtitle: " + std::string(TTF_GetError())));
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
            image = IMG_Load(Global::_PRESENT->slides[slide].image.c_str());
            if (image == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Failed to render image: " + std::string(SDL_GetError())));
            else
            {
                dst.h = Global::_HEIGHT - contentHeight - Global::_BORDERS;
                dst.w = int(float(float(dst.h) / float(image->h) * float(image->w)));
                dst.x = Global::_WIDTH - Global::_BORDERS - dst.w;
                dst.y = Global::_HEIGHT - Global::_BORDERS - dst.h;

                if (dst.w > Global::_WIDTH / 2 && Global::_PRESENT->slides[slide].points.size() != 0)
                {
                    SDL_Rect tmp = dst;

                    dst.w = Global::_WIDTH / 2;
                    dst.h = int(float(float(dst.w) / float(tmp.w) * float(tmp.h)));
                    dst.x = Global::_WIDTH - Global::_BORDERS - dst.w;
                    dst.y = Global::_HEIGHT - Global::_BORDERS - dst.h;
                }
                else if(dst.w > Global::_WIDTH - (2 * Global::_BORDERS) && Global::_PRESENT->slides[slide].points.size() == 0)
                {
                    SDL_Rect tmp = dst;

                    dst.w = Global::_WIDTH - (2 * Global::_BORDERS);
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
            text = TTF_RenderUTF8_Blended(Global::_PRESENT->font["normal"], "- ", *Global::_PRESENT->textcolor);
            if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering dash: " + std::string(TTF_GetError())));
            else
            {
                dst = { Global::_BORDERS, contentHeight, text->w, text->h };
                SDL_BlitScaled(text, NULL, surface, &dst);
                SDL_FreeSurface(text);
            }

            //Render Text
            text = TTF_RenderUTF8_Blended_Wrapped(Global::_PRESENT->font["normal"], Global::_PRESENT->slides[slide].points[i].text.c_str(), *Global::_PRESENT->textcolor, imgPosX - 2 * Global::_BORDERS - dst.x);
            if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering point: " + std::string(TTF_GetError())));
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
                //Render Arrows
                text = TTF_RenderUTF8_Blended(Global::_PRESENT->font["normal"], "> ", *Global::_PRESENT->textcolor);
                if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering arrow: " + std::string(TTF_GetError())));
                else
                {
                    dst = { Global::_BORDERS + Global::_INDENT, contentHeight, text->w, text->h };
                    SDL_BlitScaled(text, NULL, surface, &dst);
                    SDL_FreeSurface(text);
                }

                //Render Text
                text = TTF_RenderUTF8_Blended_Wrapped(Global::_PRESENT->font["normal"], Global::_PRESENT->slides[slide].points[i].subPoints[j].c_str(), *Global::_PRESENT->textcolor, imgPosX - 2 * Global::_BORDERS - dst.x);
                if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering subpoints: " + std::string(TTF_GetError())));
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
            text = TTF_RenderUTF8_Blended(Global::_PRESENT->font["footer"], "Created with BitPresent", *Global::_PRESENT->textcolor);
            if (text == NULL) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error rendering footer: " + std::string(TTF_GetError())));
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