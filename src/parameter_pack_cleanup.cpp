/**
 * @file parameter_pack_cleanup.cpp
 *
 * @author  Guy Maurel
 * @license GPL v2+
 */

#include "parameter_pack_cleanup.h"

#include "chunk.h"
#include "uncrustify.h"


void parameter_pack_cleanup()
{
   LOG_FUNC_ENTRY();

   Chunk *pc = Chunk::GetHead();

   while (pc->IsNotNullChunk())
   {
      LOG_FMT(LTOK, "%s(%d): orig line is %zu, orig col is %zu, Text() is '%s', type is %s\n",
              __func__, __LINE__, pc->GetOrigLine(), pc->GetOrigCol(), pc->Text(), get_token_name(pc->GetType()));

      // look for template
      if (pc->Is(CT_TEMPLATE))                 // Issue #3309
      {
         Chunk *template_end = pc->GetNextType(CT_SEMICOLON, pc->level);

         // look for a parameter pack
         while (pc->IsNotNullChunk())
         {
            LOG_FMT(LTOK, "%s(%d): orig line is %zu, orig col is %zu, Text() is '%s'\n",
                    __func__, __LINE__, pc->GetOrigLine(), pc->GetOrigCol(), pc->Text());

            if (pc->Is(CT_PARAMETER_PACK))
            {
               Chunk *parameter_pack = pc;

               // look for a token with the same text
               while (pc->IsNotNullChunk())
               {
                  LOG_FMT(LTOK, "%s(%d): orig line is %zu, orig col is %zu, Text() is '%s'\n",
                          __func__, __LINE__, pc->GetOrigLine(), pc->GetOrigCol(), pc->Text());

                  if (pc == template_end)
                  {
                     break;
                  }

                  if (strcmp(pc->Text(), parameter_pack->Text()) == 0)
                  {
                     pc->SetType(CT_PARAMETER_PACK);
                  }
                  pc = pc->GetNext();
               }
            }
            pc = pc->GetNext();

            if (pc == template_end)
            {
               break;
            }
         }
      }
      pc = pc->GetNext();
   }
} // parameter_pack_cleanup
