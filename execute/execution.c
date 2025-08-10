/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:48 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/10 18:07:50 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	list_len(t_cmd *list)
{
	int	i;

	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

static void	cleanup_heredoc_files(char **heredoc_files, t_cmd *cmd_list)
{
    int i;
    t_cmd *curr_cmd;
    t_redirection *curr_redir;
    char *silinecek_dosya;
    
    if (!heredoc_files)
        return;
    
    i = 0;
    while (heredoc_files[i])
    {
        silinecek_dosya = heredoc_files[i];
        
        // Geçici dosyayı dosya sisteminden sil
        unlink(silinecek_dosya);
        
        // Bu belleği işaret eden yönlendirmeleri kontrol et
        // ve uygun şekilde işle
        curr_cmd = cmd_list;
        while (curr_cmd)
        {
            curr_redir = curr_cmd->redirections;
            while (curr_redir)
            {
                if (curr_redir->infile == silinecek_dosya)
                {
                    // Seçenek A: NULL yap (dosya adına artık ihtiyaç yoksa)
                    curr_redir->infile = NULL;
                    
                    // Seçenek B: Kopya oluştur (dosya adına hala ihtiyaç varsa)
                    // curr_redir->infile = ft_strdup(silinecek_dosya);
                }
                curr_redir = curr_redir->next;
            }
            curr_cmd = curr_cmd->next;
        }
        
        // Artık orijinal belleği güvenle serbest bırakabiliriz
        free(silinecek_dosya);
        heredoc_files[i] = NULL;  // Çift serbest bırakmayı önle
        i++;
    }
    free(heredoc_files);
}

static int  has_heredoc(t_cmd *cmd_list)
{
    t_cmd           *current_cmd;
    t_redirection   *current_redir;

    current_cmd = cmd_list;
    while (current_cmd)
    {
        current_redir = current_cmd->redirections;
        while (current_redir)
        {
            if (current_redir->type == T_HEREDOC)
                return (1);
            current_redir = current_redir->next;
        }
        current_cmd = current_cmd->next;
    }
    return (0);
}

static char **preprocess_heredocs(t_cmd *cmd_list)
{
    t_cmd   *curr_cmd;
    char    **tmp_files;
    int     i;

    tmp_files = malloc(sizeof(char *) * (list_len(cmd_list) * 10 + 1));
    if (!tmp_files)
        return (NULL);
    i = 0;
    curr_cmd = cmd_list;
    while (curr_cmd)
    {
        if (!process_heredocs_in_cmd(curr_cmd, tmp_files, &i))
        {
            tmp_files[i] = NULL;
            cleanup_heredoc_files(tmp_files, cmd_list);
            return (NULL);
        }
        curr_cmd = curr_cmd->next;
    }
    tmp_files[i] = NULL;
    return (tmp_files);
}

static int	should_run_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd || cmd->redirections)
	{
		return (0);
	}
	if (ft_strcmp(cmd->cmd, "cd") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(cmd->cmd, "export") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

void	debug_heredoc_memory(char **heredoc_files, t_cmd *cmd_list)
{
	int				i;
	t_cmd			*curr_cmd;
	t_redirection	*curr_redir;

	printf("=== HEREDOC MEMORY DEBUG ===\n");
	
	if (heredoc_files)
	{
		i = 0;
		while (heredoc_files[i])
		{
			printf("heredoc_files[%d] = %p (%s)\n", i, heredoc_files[i], heredoc_files[i]);
			i++;
		}
	}
	
	curr_cmd = cmd_list;
	while (curr_cmd)
	{
		curr_redir = curr_cmd->redirections;
		while (curr_redir)
		{
			if (curr_redir->infile)
				printf("redirection infile = %p (%s)\n", curr_redir->infile, curr_redir->infile);
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
	}
	printf("=== END DEBUG ===\n");
}

void cleanup_redirections(t_redirection *redirections)
{
    t_redirection *curr;
    t_redirection *next;
    
    curr = redirections;
    while (curr)
    {
        next = curr->next;
        if (curr->infile)
            free(curr->infile);
        if (curr->outfile)
            free(curr->outfile);
        free(curr);
        curr = next;
    }
}

int ft_execute(t_envlist *env, t_cmd *cmd_list, t_token *all_tokens)
{
    char    **heredoc_files;

    heredoc_files = NULL;
    if (!cmd_list)
        return (PARSE_EXECUTE_OK);

    if (has_heredoc(cmd_list))
    {
        heredoc_files = preprocess_heredocs(cmd_list);
        if (!heredoc_files)
            return (PARSE_EXECUTE_OK);
        
        printf("HEREDOC İŞLEMİ SONRASI:\n");
        debug_heredoc_memory(heredoc_files, cmd_list);
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    
    if (list_len(cmd_list) == 1 && is_builtin(cmd_list)
        && should_run_parent_builtin(cmd_list))
    {
        if (ft_strcmp(cmd_list->cmd, "exit") == 0)
        {
            execute_builtin(cmd_list, env, 0);
            
            // Exit öncesi temizlik
            if (heredoc_files)
            {
                printf("EXIT ÖNCESİ TEMİZLİK:\n");
                debug_heredoc_memory(heredoc_files, cmd_list);
                cleanup_heredoc_files(heredoc_files, cmd_list);
            }
            return (PARSE_EXECUTE_EXIT);
        }
        execute_builtin(cmd_list, env, 0);
    }
    else
        execute_pipeline(cmd_list, env, cmd_list, all_tokens);
    
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
    
    // Son temizlik
    if (heredoc_files)
    {
        printf("SON TEMİZLİK ÖNCESİ:\n");
        debug_heredoc_memory(heredoc_files, cmd_list);
        cleanup_heredoc_files(heredoc_files, cmd_list);
        printf("TEMİZLİK TAMAMLANDI\n");
    }
        
    return (PARSE_EXECUTE_OK);
}
