using System.Threading;
using System.Threading.Tasks;
using Domain.RepositoryInterfaces;
using MediatR;

namespace Application.UseCases.Pairs.DeletePair;

public class DeletePairHandler : IRequestHandler<DeletePairRequest, DeletePairResponse>
{
    private readonly IPairRepository _pairRepository;

    public DeletePairHandler(IPairRepository pairRepository)
    {
        _pairRepository = pairRepository;
    }
        
    public async Task<DeletePairResponse> Handle(DeletePairRequest request, CancellationToken cancellationToken)
    {
        request.Validate();

        var isSuccess = await _pairRepository.DeleteAsync(request.Id, cancellationToken);

        return new DeletePairResponse(isSuccess);
    }
}